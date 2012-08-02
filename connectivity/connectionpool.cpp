#include "connectionpool.h"
#include "util/asyncconnect.h"
#include "dbconnection.h"
#include <QMutexLocker>
#include <QtDebug>
#include <QMutexLocker>

ConnectionPool::ConnectionPool(QObject *parent) :
    QObject(parent), backupConnection(0), backupCreationInProgress(false), busy(0)
{
}

ConnectionPool::~ConnectionPool()
{
    if(backupConnection!=0){
        delete backupConnection;
    }
}

void ConnectionPool::requestConnection(DbConnection *cloneOf, void *data)
{  
    busy++;

    bool servedFromBackup=false;

    if(backupConnection!=0){
        emit asyncConnectionReady(backupConnection, data, false, OciException());
        backupConnection=0;
        servedFromBackup=true;
        qDebug() << "served connection from backup";
    }

    DbConnection *newDb=cloneOf->clone();
    AsyncConnect *asyncConnect=new AsyncConnect(newDb, servedFromBackup ? 0: data, QThread::currentThread(), this);
    connect(asyncConnect, SIGNAL(connectionEstablished(AsyncConnect*,DbConnection*,void*,bool,OciException)),
             this, SLOT(asyncConnectionEstablished(AsyncConnect*,DbConnection*,void*,bool,OciException)));
    asyncConnect->start();
}

void ConnectionPool::asyncConnectionEstablished(AsyncConnect *thread, DbConnection *db, void *data, bool error, const OciException &ex)
{
    if(data==0){
        Q_ASSERT(backupConnection==0);
        if(!error){backupConnection=db;}
        backupCreationInProgress=false;
        qDebug() << "created backup connection";
    }else{
        emit asyncConnectionReady(db, data, error, ex);

        if(backupConnection==0 && !backupCreationInProgress && !error){
            createBackupConnection(db);
        }
    }

    thread->wait();
    thread->deleteLater();

    busy--;
}

void ConnectionPool::createBackupConnection(DbConnection *cloneOf)
{
    mutex.lock();//review this part

    if(backupConnection!=0 || backupCreationInProgress){
        return;
    }
    backupCreationInProgress=true;
    requestConnection(cloneOf, 0);

    mutex.unlock();
}

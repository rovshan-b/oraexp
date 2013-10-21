#include "connectionpool.h"
#include "util/asyncconnect.h"
#include "util/appconnectionmanager.h"
#include "dbconnection.h"
#include <QMutexLocker>
#include <QtDebug>
#include <QMutexLocker>
#include <QApplication>

ConnectionPool::ConnectionPool(QObject *parent) :
    QObject(parent), backupConnection(0), backupCreationInProgress(false), busy(0)
{
}

ConnectionPool::~ConnectionPool()
{
    if(backupConnection!=0){
        AppConnectionManager::deleteConnection(backupConnection); //it is not registered
    }
}

void ConnectionPool::requestConnection(DbConnection *cloneOf, void *data)
{  
    //Q_ASSERT(QApplication::instance()->thread() == QThread::currentThread());

    busy++;
    emit working(true);

    bool servedFromBackup=false;

    if(backupConnection!=0){
        emit asyncConnectionReady(backupConnection, data, false, OciException());
        backupConnection=0;
        servedFromBackup=true;
        qDebug() << "served connection from backup";

        //will be creating backup connection below
        backupCreationInProgress = true;
    }

    DbConnection *newDb=cloneOf->clone();
    AsyncConnect *asyncConnect=new AsyncConnect(newDb, servedFromBackup ? 0 : data, QThread::currentThread(), this);
    connect(asyncConnect, SIGNAL(connectionEstablished(AsyncConnect*,DbConnection*,void*,bool,OciException)),
             this, SLOT(asyncConnectionEstablished(AsyncConnect*,DbConnection*,void*,bool,OciException)));
    asyncConnect->start();
}

DbConnection *ConnectionPool::getBackupConnection() const
{
    return this->backupConnection;
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
    emit working(false);
}

void ConnectionPool::createBackupConnection(DbConnection *cloneOf)
{
    QMutexLocker locker(&mutex);

    if(backupConnection!=0 || backupCreationInProgress){
        return;
    }
    backupCreationInProgress=true;
    requestConnection(cloneOf, 0);
}

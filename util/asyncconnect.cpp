#include "asyncconnect.h"
#include "connectivity/dbconnection.h"
#include "util/queryqueuemanager.h"

#include <iostream>
using namespace std;

AsyncConnect::AsyncConnect(DbConnection *db, void *data, QThread *guiThread, QObject *parent) :
    QThread(parent), db(db), data(data), guiThread(guiThread)
{

}

AsyncConnect::~AsyncConnect()
{
    cout << "deleted AsyncConnect" << endl;
}

void AsyncConnect::run()
{
    try{
        db->connect();

        db->getQueueManager()->moveToThread(guiThread);

        emit connectionEstablished(this, db, data);
    }catch(OciException &ex){
        emit connectionEstablished(this, db, data, true, ex);
    }
}

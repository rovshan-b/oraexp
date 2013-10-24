#include "asyncreconnect.h"
#include "connectivity/dbconnection.h"
#include "util/queryutil.h"

AsyncReconnect::AsyncReconnect(DbConnection *db, QObject *parent) :
    QThread(parent), db(db)
{
}

void AsyncReconnect::run()
{
    //check if already connected
    try{
        db->executeQueryAndCleanup(QueryUtil::getQuery("ping_query", db));

        qDebug("already connected");

        emit reconnected(db); //already connected
        return;
    }catch(OciException &){
        //not connected, proceed to below code
    }

    qDebug("reconnecting");

    try{
        db->reconnect();

        emit reconnected(db);
    }catch(OciException &ex){
        emit reconnected(db, true, ex);
    }
}

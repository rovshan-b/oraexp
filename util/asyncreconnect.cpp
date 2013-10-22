#include "asyncreconnect.h"
#include "connectivity/dbconnection.h"

AsyncReconnect::AsyncReconnect(DbConnection *db, QObject *parent) :
    QThread(parent), db(db)
{
}

void AsyncReconnect::run()
{
    try{
        db->disconnect();
        db->connect();

        emit reconnected(db);
    }catch(OciException &ex){
        emit reconnected(db, true, ex);
    }
}

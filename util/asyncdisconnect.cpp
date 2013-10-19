#include "asyncdisconnect.h"
#include "connectivity/dbconnection.h"
#include <QDebug>

AsyncDisconnect::AsyncDisconnect(DbConnection *db, QObject *parent) :
    QThread(parent), db(db)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void AsyncDisconnect::run()
{
    db->disconnect();

    delete db;

    emit disconnected(db);
}

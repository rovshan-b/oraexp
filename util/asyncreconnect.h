#ifndef ASYNCRECONNECT_H
#define ASYNCRECONNECT_H

#include <QThread>
#include "connectivity/ociexception.h"

class DbConnection;

class AsyncReconnect : public QThread
{
    Q_OBJECT
public:
    explicit AsyncReconnect(DbConnection *db, QObject *parent);
    
protected:
    virtual void run();

signals:
    void reconnected(DbConnection *db, bool error=false, const OciException &ex=OciException());

private:
    DbConnection *db;
    
};

#endif // ASYNCRECONNECT_H

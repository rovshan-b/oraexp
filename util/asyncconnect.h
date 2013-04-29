#ifndef ASYNCCONNECT_H
#define ASYNCCONNECT_H

#include <QThread>
#include "connectivity/ociexception.h"

class DbConnection;

class AsyncConnect : public QThread
{
    Q_OBJECT
public:
    explicit AsyncConnect(DbConnection *db, void *data, QThread *guiThread, QObject *parent);
    ~AsyncConnect();

protected:
    virtual void run();

signals:
    void connectionEstablished(AsyncConnect *thread, DbConnection *db, void *data, bool error=false, const OciException &ex=OciException());

private:
    DbConnection *db;
    void *data;
    QThread *guiThread;
};

#endif // ASYNCCONNECT_H

#ifndef ASYNCDISCONNECT_H
#define ASYNCDISCONNECT_H

#include <QThread>

class DbConnection;

class AsyncDisconnect : public QThread
{
    Q_OBJECT
public:
    explicit AsyncDisconnect(DbConnection *db, QObject *parent = 0);

signals:
    void disconnected(DbConnection *db);
    
protected:
    virtual void run();

private:
    DbConnection *db;
    
};

#endif // ASYNCDISCONNECT_H

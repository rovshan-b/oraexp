#ifndef ASYNCDISCONNECT_H
#define ASYNCDISCONNECT_H

#include <QRunnable>
#include <QObject>

class DbConnection;

class AsyncDisconnect : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit AsyncDisconnect(DbConnection *db);

signals:
    void disconnected(DbConnection *db);
    
protected:
    virtual void run();

private:
    DbConnection *db;
    
};

#endif // ASYNCDISCONNECT_H

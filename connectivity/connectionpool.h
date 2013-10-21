#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H

#include <QObject>
#include "ociexception.h"
#include <QQueue>
#include <QMutex>

class AsyncConnect;
class DbConnection;

class ConnectionPool : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionPool(QObject *parent = 0);
    ~ConnectionPool();

    void requestConnection(DbConnection *cloneOf, void *data);
    bool isBusy(){return this->busy>0;}

    DbConnection *getBackupConnection() const;

signals:
    void working(bool isWorking);

signals:
    void asyncConnectionReady(DbConnection *db, void *data, bool error, const OciException &ex);

private slots:
    void asyncConnectionEstablished(AsyncConnect *thread, DbConnection *db, void *data, bool error, const OciException &ex);

private:
    DbConnection *backupConnection;

    QMutex mutex;
    bool backupCreationInProgress;
    int busy;

    void createBackupConnection(DbConnection *cloneOf);

};

#endif // CONNECTIONPOOL_H

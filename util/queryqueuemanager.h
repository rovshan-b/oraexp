#ifndef QUERYQUEUEMANAGER_H
#define QUERYQUEUEMANAGER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include "util/queryexectask.h"
#include "connectivity/queryresult.h"

class QueryRunner;
class DbConnection;

class QueryQueueManager : public QObject
{
    Q_OBJECT
public:
    explicit QueryQueueManager(DbConnection *db, QObject *parent = 0);

    void enqueueQuery(const QueryExecTask &task);

    bool isBusy(){return isWorking;}

signals:
    void queueEmpty();

private slots:
    void runnerThreadFinished();
    void fetchCompleted(const QueryResult &result, const QueryExecTask &task, int resultsetIx, int resultsetCount);

private:
    void processQueue();
    void notifyQueryEndMonitor(const QueryExecTask &task);
    void invokeFetchCompletedSlot(const QueryExecTask &task, int resultsetIx);

    DbConnection *db;

    QQueue<QueryExecTask> queryQueue;

    volatile bool isWorking;

    QMutex mutex;

};

#endif // QUERYQUEUEMANAGER_H

#include "queryqueuemanager.h"
#include "connectivity/queryrunner.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "connectivity/ociexception.h"
#include "connectivity/recordfetcherthread.h"
#include "util/queryutil.h"
#include <memory>
#include <QMetaObject>
#include <QThreadPool>
#include <QDebug>

using namespace std;

QueryQueueManager::QueryQueueManager(DbConnection *db, QObject *parent) :
    QObject(parent), db(db), isWorking(false)
{

}

void QueryQueueManager::enqueueQuery(const QueryExecTask &task)
{
    queryQueue.enqueue(task);
    if(!isWorking){
        processQueue();
    }
}

void QueryQueueManager::processQueue()
{
    mutex.lock();
    Q_ASSERT(!isWorking);
    isWorking=true;

    if(queryQueue.isEmpty()){
        isWorking=false;
        mutex.unlock();
        emit queueEmpty();
    }else{
        QueryExecTask task=queryQueue.dequeue();
        if(task.requester==0){
            qDebug() << "Object deleted with query registered in queue (QueryQueueManager::processQueue)";
            notifyQueryEndMonitor(task);
            isWorking=false;
            mutex.unlock();
            processQueue();
            return;
        }

        int activeThreadCount=QThreadPool::globalInstance()->activeThreadCount();
        int maxThreadCount=QThreadPool::globalInstance()->maxThreadCount();

        if(activeThreadCount>=maxThreadCount){
            qDebug() << "INCREASING MAX THREAD COUNT IN POOL TO" << (maxThreadCount+1);
            QThreadPool::globalInstance()->setMaxThreadCount(maxThreadCount+1);
        }

        QueryRunner *runnable=new QueryRunner(db, task, this);
        QThreadPool::globalInstance()->start(runnable);
    }
}

void QueryQueueManager::fetchCompleted(const QueryResult &result, const QueryExecTask &task)
{
    if(!task.fetchCompletedSlotName.isEmpty() && !result.hasError){
        if(task.requester!=0){
            QMetaObject::invokeMethod(task.requester, task.fetchCompletedSlotName.toStdString().c_str(), Qt::DirectConnection,
                                  Q_ARG(QString, task.taskName));
        }else{
            qDebug() << "Object deleted with query registered in queue (QueryQueueManager::fetchCompleted)";
            if(result.statement!=0){
                delete result.statement;
            }
        }
    }

    notifyQueryEndMonitor(task);

    runnerThreadFinished();
}

void QueryQueueManager::runnerThreadFinished()
{
    isWorking=false;
    if(queryQueue.isEmpty()){
        mutex.unlock();
        emit queueEmpty();
    }else{
        mutex.unlock();
        processQueue();
    }
}

void QueryQueueManager::notifyQueryEndMonitor(const QueryExecTask &task)
{
    if(task.queryEndMonitor!=0){
        QMetaObject::invokeMethod(task.queryEndMonitor, "queryExecTaskCompleted", Qt::DirectConnection,
                              Q_ARG(QString, task.taskName));
    }
}

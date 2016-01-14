#include "queryrunner.h"
#include "dbconnection.h"
#include "connection.h"
#include "statement.h"
#include "fetchresult.h"
#include "util/strutil.h"
#include <QDebug>

QueryRunner::QueryRunner(DbConnection *db, const QueryExecTask &task, QObject *parent) :
    db(db), task(task), parent(parent)
{

}

QueryRunner::~QueryRunner()
{

}

void QueryRunner::run()
{
   QueryResult result;

   try{
        if(!checkPointer(task.requester, "before executing query")){
            result.hasError=true;
            invokeFetchCompletedSlot(result, task);
            return;
        }

        if(!task.isUserQuery){
            setDbLinkName(task.query, task.dbLinkName);
        }

        result=db->executeQuery(task.query,
                                task.params,
                                task.retrieveResultsetAsBindParameter,
                                task.retrieveScrollableResultsets);
        if(result.statement!=0 && result.statement->isAnonymousBlock()){
            result.dbmsOutput=db->retrieveDbmsOutput();
            qDebug() << result.dbmsOutput;
        }
    }catch(OciException &ex){
        result.hasError=true;
        result.exception=ex;
        result.dbmsOutput=db->retrieveDbmsOutput();
    }

    result.taskName=task.taskName;
    result.requester=task.requester;

    if(checkPointer(task.requester, "query completed")){
        QMetaObject::invokeMethod(task.requester, task.queryCompletedSlotName.toStdString().c_str(), Qt::QueuedConnection, Q_ARG(QueryResult, result));
    }

    if(!result.hasError && !task.fetchSlotName.isEmpty()){
        fetchResultsets(result, task);
    }else{
        invokeFetchCompletedSlot(result, task);
    }
}

void QueryRunner::fetchResultsets(QueryResult &result, const QueryExecTask &task)
{
    QScopedPointer<Statement> stmt(result.statement);

    int rsCount = stmt->rsCount();
    Q_ASSERT(rsCount>0);

    for(int i=0; i<rsCount; ++i){
        fetchResultset(result, task, i, rsCount);
    }

    result.statement=0;
}

void QueryRunner::fetchResultset(QueryResult &result, const QueryExecTask &task, int rsIx, int rsCount)
{
    Resultset *rs=result.statement->rsAt(rsIx);
    FetchResult fetchResult;
    fetchResult.resultsetIx=rsIx;
    fetchResult.columnTitles=rs->getColumnTitles();
    fetchResult.taskName=task.taskName;

    //const char *fetchSlotName=;
    int columnCount=rs->getColumnCount();

    bool couldInvoke;
    try{
        rs->beginFetchRows();
        while(rs->moveNext()){

            //if(!checkPointer(task.requester, "fetching rows")){
            //    break;
            //}

            QList<QString> oneRow;
            oneRow.reserve(columnCount);

            for(int i=0; i<columnCount; i++){
                oneRow.append(rs->getAsString(i+1));
            }

            fetchResult.hasError=false;
            fetchResult.oneRow=oneRow;

            couldInvoke=QMetaObject::invokeMethod(task.requester, task.fetchSlotName.toStdString().c_str(), Qt::QueuedConnection, Q_ARG(FetchResult, fetchResult));
            if(!couldInvoke){
                break;
            }
        }
        rs->endFetchRows();
    }catch(OciException &ex){
        fetchResult.hasError=true;
        fetchResult.exception=ex;
        fetchResult.oneRow=QList<QString>();

        checkPointer(task.requester, "fetch exception");
        QMetaObject::invokeMethod(task.requester, task.fetchSlotName.toStdString().c_str(), Qt::QueuedConnection, Q_ARG(FetchResult, fetchResult));
    }

    checkPointer(task.requester, "completed fetching rows");
    invokeFetchCompletedSlot(result, task, rsIx, rsCount);
}

bool QueryRunner::checkPointer(QObject *object, const QString &context)
{
    if(object==0){
        qDebug() << "Object deleted with query registered in queue (QueryRunner::checkPointer)" << "Context:" << context;
    }

    return object!=0;
}

void QueryRunner::invokeFetchCompletedSlot(const QueryResult &result, const QueryExecTask &task, int resultsetIx, int resultsetCount)
{
    QMetaObject::invokeMethod(this->parent, "fetchCompleted", Qt::QueuedConnection,
                                  Q_ARG(QueryResult, result),
                                  Q_ARG(QueryExecTask, task),
                                  Q_ARG(int, resultsetIx),
                                  Q_ARG(int, resultsetCount));
}

#include "sequentialqueryrunner.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include "util/queryscheduler.h"
#include <QtGui>
#include <QDebug>

SequentialQueryRunner::SequentialQueryRunner(QObject *parent) :
    QObject(parent),
    queryScheduler(0),
    lastErrorAction(SequantialExecutionErrorDialog::IgnoreCurrent),
    busy(false)
{

}

void SequentialQueryRunner::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler = queryScheduler;
}

void SequentialQueryRunner::execute(const QString &query, QWidget *parentWidget)
{
    Q_ASSERT(queryScheduler);

    this->parentWidget = parentWidget;
    stopped=false;
    ignoreAllErrors = false;
    errorCodesToIgnore.clear();

    queries.clear();
    queryPositions.clear();

    int subqueryStartPos=0, subqueryEndPos=0;
    int currentPos;

    while(subqueryStartPos!=-1 && subqueryEndPos!=-1){
        currentPos = subqueryEndPos;
        PlSqlParseHelper::getNextQueryPos(query, currentPos, &subqueryStartPos, &subqueryEndPos);

        if(subqueryStartPos!=-1 && subqueryEndPos!=-1){
            QString subquery = query.mid(subqueryStartPos, subqueryEndPos-subqueryStartPos-1);
            queries.append(subquery);
            queryPositions.append(qMakePair(subqueryStartPos, subqueryEndPos));
        }
    }

    if(queries.size()>0){
        this->busy=true;
        executeNextQuery();
    }
}

void SequentialQueryRunner::stop()
{
    this->stopped=true;
}

void SequentialQueryRunner::executeNextQuery()
{
    if(queries.isEmpty() || stopped){
        emitCompletedSignal();
        return;
    }

    currentQuery = queries.takeFirst();
    QPair<int,int> positions = queryPositions.takeFirst();
    currentQueryStartPos = positions.first;

    emit beforeExecute(currentQuery, positions.first, positions.second);

    queryScheduler->enqueueQuery(QString("$%1").arg(currentQuery),
                                 QList<Param*>(),
                                 this,
                                 "sequential_runner_task",
                                 "queryCompleted");
}

void SequentialQueryRunner::emitCompletedSignal()
{
    emit completed();

    queries.clear();
    queryPositions.clear();

    this->busy=false;
}

void SequentialQueryRunner::queryCompleted(const QueryResult &result)
{
    emit queryResultAvailable(result);

    if(result.hasError && !ignoreAllErrors && !errorCodesToIgnore.contains(result.exception.getErrorCode()) && !stopped){
        SequantialExecutionErrorDialog dialog(tr("Error occured"),
                                              result.exception,
                                              currentQuery,
                                              this->parentWidget);
        dialog.setErrorAction(lastErrorAction);

        if(!dialog.exec()){ //dialog was closed by pressing close button or by pressing Escape key
            emitCompletedSignal();
            return;
        }

        lastErrorAction=dialog.getErrorAction();

        switch(lastErrorAction){
        case SequantialExecutionErrorDialog::IgnoreAll:
            ignoreAllErrors = true;
            break;
        case SequantialExecutionErrorDialog::IgnoreCurrent:
            break;
        case SequantialExecutionErrorDialog::IgnoreCurrentCode:
            errorCodesToIgnore.append(result.exception.getErrorCode());
            break;
        case SequantialExecutionErrorDialog::Abort:
            emitCompletedSignal();
            return;
            break;
        default: //unhandled case
            Q_ASSERT(false);
            break;
        }
    }

    executeNextQuery();
}

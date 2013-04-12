#include "sequentialqueryrunner.h"
#include "code_parser/plsql/plsqlparsehelper.h"
#include "util/queryscheduler.h"
#include "util/widgethelper.h"
#include "dialogs/descriptiveerrordialog.h"
#include <QtGui>
#include <QDebug>

SequentialQueryRunner::SequentialQueryRunner(QObject *parent) :
    QObject(parent),
    queryScheduler(0)
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
        executeNextQuery();
    }
}

void SequentialQueryRunner::executeNextQuery()
{
    if(queries.isEmpty()){
        emit completed();
        return;
    }

    currentQuery = queries.takeFirst();
    QPair<int,int> positions = queryPositions.takeFirst();

    emit beforeExecute(currentQuery, positions.first, positions.second);

    queryScheduler->enqueueQuery(QString("$%1").arg(currentQuery),
                                 QList<Param*>(),
                                 this,
                                 "sequential_runner_task",
                                 "queryCompleted");
}

void SequentialQueryRunner::queryCompleted(const QueryResult &result)
{
    emit queryResultAvailable(result);

    if(result.hasError){
        QString errorMessage = result.exception.getErrorMessage();

        DescriptiveErrorDialog dialog(tr("Error occured"),
                                      errorMessage,
                                      currentQuery,
                                      0,
                                      this->parentWidget);

        int colonIx = errorMessage.indexOf(':');
        QString errorPrefix = colonIx!=-1 ? errorMessage.left(colonIx) : QString::number(result.exception.getErrorCode());



        QVBoxLayout *radioLayout = new QVBoxLayout();

        QRadioButton ignoreAllRadio(tr("Ignore all errors"));
        QRadioButton ignoreCurrentRadio(tr("Ignore this error"));
        ignoreCurrentRadio.setChecked(true);
        QRadioButton ignoreCurrentCodeRadio(tr("Ignore all occurences of error %1").arg(errorPrefix));
        QRadioButton abortRadio(tr("Abort"));

        radioLayout->addWidget(&ignoreAllRadio);
        radioLayout->addWidget(&ignoreCurrentRadio);
        radioLayout->addWidget(&ignoreCurrentCodeRadio);
        radioLayout->addWidget(&abortRadio);

        QGroupBox *groupBox = WidgetHelper::createGroupBox(radioLayout, tr("Select action"));

        dialog.addWidget(groupBox);
        dialog.exec();

        emit completed();

        return;
    }

    executeNextQuery();
}

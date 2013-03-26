#include "datacomparerhelper.h"
#include "datacomparerthread.h"
#include "beans/datacomparisonoptions.h"

DataComparerHelper::DataComparerHelper(const QString &sourceSchema,
                                       IQueryScheduler *sourceScheduler,
                                       const QString &targetSchema,
                                       IQueryScheduler *targetScheduler,
                                       DbTreeModel *model,
                                       DataOperationOptions *options,
                                       const QHash<QString, TableInfoForDataComparison> &tableOptions, QObject *parent) :
    DataOperationHelper(sourceSchema,
                        sourceScheduler,
                        targetSchema,
                        targetScheduler,
                        model,
                        options,
                        tableOptions,
                        parent)
{
}

bool DataComparerHelper::canStart()
{
    DataComparisonOptions *compareOptions = static_cast<DataComparisonOptions*>(options);
    if(!compareOptions->inserts && !compareOptions->updates && !compareOptions->deletes){
        return false;
    }

    return true;
}


void DataComparerHelper::startWorkerThread(const QString &tableName)
{
    Q_ASSERT(workerThread==0);

    currentTableName = tableName;
    emit compareInfoAvailable(DataCompareInfo(tableName, tr("Starting")));

    workerThread=new DataComparerThread(sourceSchema, sourceScheduler->getDb(),
                                          targetSchema, targetScheduler->getDb(),
                                          tableName, options, tableOptions.value(tableName),
                                          this);

    connect(workerThread, SIGNAL(statusChanged(QString)), this, SIGNAL(statusChanged(QString)));
    connect(workerThread, SIGNAL(compareInfoAvailable(DataCompareInfo)), this, SIGNAL(compareInfoAvailable(DataCompareInfo)));
    connect(workerThread, SIGNAL(comparisonCompleted()), this, SLOT(tableComparisonCompleted()));
    connect(workerThread, SIGNAL(compareError(QString,OciException)), this, SLOT(tableComparisonError(QString,OciException)));

    workerThread->start();
}

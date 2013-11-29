#include "dataoperationhelper.h"
#include "navtree/dbtreeitem.h"
#include "util/queryutil.h"
#include "util/strutil.h"
#include "connectivity/statement.h"
#include "dataoperationthread.h"

DataOperationHelper::DataOperationHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler, const QString &targetSchema, IQueryScheduler *targetScheduler, DbTreeModel *model, DataOperationOptions *options, const QHash<QString, TableInfoForDataComparison> &tableOptions, QObject *parent) :
    QObject(parent),
    sourceSchema(sourceSchema),
    sourceScheduler(sourceScheduler),
    targetSchema(targetSchema),
    targetScheduler(targetScheduler),
    model(model),
    options(options),
    tableOptions(tableOptions),
    workerThread(0)
{
    connect(model, SIGNAL(childrenPopulated(QModelIndex)), this, SLOT(childrenPopulated(QModelIndex)));
    connect(model, SIGNAL(childrenPopulateError(QModelIndex,OciException)), this, SLOT(childrenPopulateError(QModelIndex,OciException)));
}

DataOperationHelper::~DataOperationHelper()
{
    delete options;

    deleteWorkerThread();
}

void DataOperationHelper::compare()
{
    emit statusChanged(tr("Starting..."));

    if(!canStart()){
        emitCompletedSignal();
        return;
    }

    currentItemIxToCompare = 0;
    QStringList tableNames = getItemsToCompare();

    if(tableNames.isEmpty()){
        emitCompletedSignal();
        return;
    }

    sortTableNames(tableNames);
}

void DataOperationHelper::stop()
{
    IDbObjectCompareHelper::stop();

    if(workerThread!=0){
        workerThread->stop();
    }
}

void DataOperationHelper::startToCompare()
{
    loadTableColumns();
}

QStringList DataOperationHelper::getItemsToCompare() const
{
    QModelIndex tablesIndex=model->getChildIndex(QModelIndex(), DbTreeModel::Tables);

    QStringList tableNames;

    int rowCount=model->rowCount(tablesIndex);
    QModelIndex childIndex;
    DbTreeItem *item;
    for(int i=0; i<rowCount; ++i){
        childIndex=model->index(i, 0, tablesIndex);
        item=static_cast<DbTreeItem *>(childIndex.internalPointer());
        if(item->checkState()!=Qt::Unchecked && item->canGenerateDdlForItem()){
            tableNames.append(item->itemName());
        }
    }

    return tableNames;
}

void DataOperationHelper::fillItemsToCompare(const QStringList &tableNames)
{
    Q_ASSERT(itemsToCompare.isEmpty() && currentItemIxToCompare==0);

    QModelIndex tablesIndex=model->getChildIndex(QModelIndex(), DbTreeModel::Tables);

    for(int i=0; i<tableNames.size(); ++i){
        const QString &soureTableName = tableNames.at(i);
        QModelIndex foundIndex = model->findByName(tablesIndex, soureTableName);
        Q_ASSERT(foundIndex.isValid());
        itemsToCompare.append(foundIndex);
        //TableInfoForDataComparison info = tableOptions.value(soureTableName);
        //targetTableNames.append(info.targetTableName.isEmpty() ? soureTableName : info.targetTableName);
    }
}

void DataOperationHelper::sortTableNames(const QStringList &tableNames)
{
    //emit statusChanged(tr("Determining compare order..."));

    sourceScheduler->enqueueQuery("sort_referencing_tables",
                                  QList<Param*>()
                                  << new Param("table_names", tableNames)
                                  << new Param("table_count", tableNames.size())
                                  << new Param("owner", sourceSchema),
                                  this,
                                  "sort_referencing_tables",
                                  "tableSortQueryCompleted",
                                  "tableSortRecordFetched",
                                  "tableSortFetchCompleted",
                                  true);
}

void DataOperationHelper::tableSortQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        subComparisonError("sort_referencing_tables_query", result.exception);
    }
}

void DataOperationHelper::tableSortRecordFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        subComparisonError("sort_referencing_tables_fetch", fetchResult.exception);
        return;
    }

    QString tables = fetchResult.oneRow.at(0);
    bool fullySorted = fetchResult.oneRow.at(1).toInt()==1;
    QString notSortedTables = fetchResult.oneRow.at(2);

    Q_ASSERT(fullySorted || !notSortedTables.isEmpty());

    QStringList sourceTableNames = tables.split(',', QString::SkipEmptyParts);
    fillItemsToCompare(sourceTableNames);

    QStringList notSortedTableNames = notSortedTables.split(',', QString::SkipEmptyParts);
    QStringList notSortedTargetTableNames;
    foreach(const QString &s, notSortedTableNames){
        const TableInfoForDataComparison &info = tableOptions.value(s);
        notSortedTargetTableNames.append(info.targetTableName.isEmpty() ? s : info.targetTableName);
    }

    if(options->disableRefConstraints==DataOperationOptions::Auto){
        tableNamesToDisableConstraints = joinEnclosed(notSortedTargetTableNames, ",", "'");
    }else if(options->disableRefConstraints==DataOperationOptions::Disable){
        tableNamesToDisableConstraints = joinEnclosed(sourceTableNames, ",", "'");
    }else{
        tableNamesToDisableConstraints = "";
    }

    this->needToDisableRefConstraints = (options->disableRefConstraints==DataOperationOptions::Disable ||
                                         options->disableRefConstraints==DataOperationOptions::DisableForAll)
                                        ||
                                        (options->disableRefConstraints==DataOperationOptions::Auto && !fullySorted);

    if(needToDisableRefConstraints){
        disableRefConstraints();
    }else{
        startToCompare();
    }
}

void DataOperationHelper::tableSortFetchCompleted(const QString &)
{
}

void DataOperationHelper::loadTableColumns()
{
    if(allCompared()){
        currentItemIxToCompare = 0;
        //fillItemsToCompare();
        this->objectCount = itemsToCompare.size();
        emit objectCountDetermined(itemsToCompare.size());
        compareNextItem();
    }else{
        const QModelIndex &tableIndex=itemsToCompare.at(currentItemIxToCompare);
        DbTreeItem *tableItem=static_cast<DbTreeItem *>(tableIndex.internalPointer());
        if(!tableItem->areChildrenPopulated()){
            model->loadChildItems(tableIndex);
        }else{
            QModelIndex columnsIndex=model->getChildIndex(tableIndex, DbTreeModel::Columns);
            DbTreeItem *columnsItem=static_cast<DbTreeItem *>(columnsIndex.internalPointer());
            if(!columnsItem->areChildrenPopulated()){
                emit statusChanged(tr("Loading column list for table %1...").arg(tableItem->itemName()));
                model->loadChildItems(columnsIndex);
            }else{
                //fill column list and remove item from queue
                TableInfoForDataComparison tableInfo=tableOptions.value(tableItem->itemName());

                QList<QModelIndex> columnIndexes = model->getChildIndexes(columnsIndex, DbTreeModel::Column);
                foreach(QModelIndex columnIndex, columnIndexes){
                    if(model->isCheckable(columnIndex) && model->data(columnIndex, Qt::CheckStateRole)==Qt::Checked){
                        tableInfo.addColumnToCompare(model->itemName(columnIndex));
                    }
                }

                tableOptions[tableItem->itemName()]=tableInfo;

                ++currentItemIxToCompare;

                loadTableColumns(); //load next items
            }
        }
    }
}

void DataOperationHelper::compareNextItem()
{
    if(allCompared() || this->stopped){
        if(needToDisableRefConstraints){
            enableRefConstraints();
        }else{
            emitCompletedSignal();
        }
    }else{
        QModelIndex tableIndex=itemsToCompare.at(currentItemIxToCompare++);
        DbTreeItem *tableItem=static_cast<DbTreeItem *>(tableIndex.internalPointer());
        Q_ASSERT(tableItem);

        emit statusChanged(tr("Comparing data for table %1").arg(tableItem->itemName()));

        QString tableName = tableItem->itemName();

        currentTableName = tableName;
        emit compareInfoAvailable(DataCompareInfo(tableName, tr("Starting")));

        startWorkerThread(tableName);
    }
}

void DataOperationHelper::subComparisonError(const QString &taskName, const OciException &exception)
{
    emit compareInfoAvailable(DataCompareInfo(currentTableName, tr("Error:%1").arg(exception.getErrorMessage())));
    emit comparisonError(taskName, exception);
    this->deleteLater();
}

void DataOperationHelper::emitCompletedSignal()
{
    emit completed();

    this->deleteLater();
}

void DataOperationHelper::childrenPopulated(const QModelIndex &)
{
    loadTableColumns();
}

void DataOperationHelper::childrenPopulateError(const QModelIndex &, const OciException &exception)
{
    subComparisonError("populate_tree_child_nodes", exception);
}

void DataOperationHelper::tableComparisonCompleted()
{
    emit chunkCompleted(1);
    emit tableCompareSuccess(this->currentTableName);

    deleteWorkerThread();
    compareNextItem();
}

void DataOperationHelper::tableComparisonError(const QString &taskName, const OciException &exception)
{
    if(options->skipOnError && !allCompared()){
        emit compareInfoAvailable(DataCompareInfo(currentTableName, tr("Error:%1").arg(exception.getErrorMessage())));
        tableComparisonCompleted();
        return;
    }

    deleteWorkerThread();

    if(needToDisableRefConstraints &&
            options->comparisonMode==DataOperationOptions::UpdateDatabase){
        lastExceptionTaskName=taskName;
        lastException=exception;
        enableRefConstraints();
    }else{
        subComparisonError(taskName, exception);
    }
}

void DataOperationHelper::deleteWorkerThread()
{
    if(!workerThread){
        return;
    }

    workerThread->wait();
    delete workerThread;
    workerThread=0;
}

void DataOperationHelper::disableRefConstraints()
{
    emit statusChanged(tr("Disabling referential constraints"));

    QString disableQuery = QueryUtil::getQuery(this->tableNamesToDisableConstraints.isEmpty() ?
                                               "disable_all_ref_constraints" : "disable_ref_constraints",
                                               targetScheduler->getDb());
    disableQuery.replace("{owner}", this->targetSchema);
    disableQuery.replace("{table_names}", this->tableNamesToDisableConstraints);

    if(options->comparisonMode==DataOperationOptions::UpdateDatabase){
        targetScheduler->enqueueQuery(QString("$%1").arg(disableQuery),
                                      QList<Param*>(),
                                      this,
                                      "disable_all_ref_constraints",
                                      "refConstraintsDisabled");
    }else{
        emit queryTextAvailable(QString("%1/\n").arg(disableQuery));
        startToCompare();
    }
}

void DataOperationHelper::enableRefConstraints()
{
    emit statusChanged(tr("Enabling referential constraints"));

    enableRefContraintsQuery = QueryUtil::getQuery(this->tableNamesToDisableConstraints.isEmpty() ?
                                                   "enable_all_ref_constraints" : "enable_ref_constraints",
                                                   targetScheduler->getDb());
    enableRefContraintsQuery.replace("{owner}", this->targetSchema);
    enableRefContraintsQuery.replace("{table_names}", this->tableNamesToDisableConstraints);

    if(options->comparisonMode==DataOperationOptions::UpdateDatabase){
        targetScheduler->enqueueQuery(QString("$%1").arg(enableRefContraintsQuery),
                                  QList<Param*>(),
                                  this,
                                  "enable_all_ref_constraints",
                                  "refConstraintsEnabled");
    }else{
        emit queryTextAvailable(QString("%1\n/").arg(enableRefContraintsQuery));
        emitCompletedSignal();
    }
}

bool DataOperationHelper::allCompared() const
{
    return currentItemIxToCompare >= itemsToCompare.size();
}

void DataOperationHelper::refConstraintsDisabled(const QueryResult &result)
{
    delete result.statement;

    if(result.hasError){
        subComparisonError(result.taskName, result.exception);
    }else{
        startToCompare();
    }
}

void DataOperationHelper::refConstraintsEnabled(const QueryResult &result)
{
    delete result.statement;

    if(result.hasError){
        OciException ex=result.exception;

        if(options->comparisonMode==DataOperationOptions::UpdateDatabase){
            ex.addToErrorMessage(tr("Operation completed successfully but application failed to enable referential constraints.\nThey need to be enabled manually with provided query.\n"));
        }

        if(!lastException.isEmpty()){
            lastException.addToErrorMessage(tr("Referential constraints must be enabled manually with provided query."), false);
        }

        emit queryTextAvailable(enableRefContraintsQuery);

        subComparisonError(lastExceptionTaskName.isEmpty() ? result.taskName : lastExceptionTaskName, lastException.isEmpty() ? ex : lastException);
    }else{
        if(lastExceptionTaskName.isEmpty()){
            emitCompletedSignal();
        }else{
            subComparisonError(lastExceptionTaskName, lastException);
        }
    }
}

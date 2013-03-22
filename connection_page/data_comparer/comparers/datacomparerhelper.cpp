#include "datacomparerhelper.h"
#include "navtree/dbtreeitem.h"
#include "datacomparerthread.h"
#include "util/queryutil.h"
#include "util/strutil.h"
#include "connectivity/statement.h"

DataComparerHelper::DataComparerHelper(const QString &sourceSchema,
                                       IQueryScheduler *sourceScheduler,
                                       const QString &targetSchema,
                                       IQueryScheduler *targetScheduler,
                                       DbTreeModel *model,
                                       DataComparisonOptions *options,
                                       const QHash<QString, TableInfoForDataComparison> &tableOptions, QObject *parent) : QObject(parent) ,
    sourceSchema(sourceSchema),
    sourceScheduler(sourceScheduler),
    targetSchema(targetSchema),
    targetScheduler(targetScheduler),
    model(model),
    options(options),
    tableOptions(tableOptions),
    comparerThread(0)
{
    connect(model, SIGNAL(childrenPopulated(QModelIndex)), this, SLOT(childrenPopulated(QModelIndex)));
    connect(model, SIGNAL(childrenPopulateError(QModelIndex,OciException)), this, SLOT(childrenPopulateError(QModelIndex,OciException)));
}

DataComparerHelper::~DataComparerHelper()
{
    delete options;

    deleteComparerThread();
}

void DataComparerHelper::compare()
{
    emit statusChanged(tr("Starting..."));

    if(!options->inserts && !options->updates && !options->deletes){
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

void DataComparerHelper::startToCompare()
{
    loadTableColumns();
}

QStringList DataComparerHelper::getItemsToCompare() const
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

void DataComparerHelper::fillItemsToCompare(const QString &tables)
{
    Q_ASSERT(itemsToCompare.isEmpty() && currentItemIxToCompare==0);

    QStringList sourceTableNames = tables.split(',', QString::SkipEmptyParts);
    QStringList targetTableNames;

    QModelIndex tablesIndex=model->getChildIndex(QModelIndex(), DbTreeModel::Tables);

    for(int i=0; i<sourceTableNames.size(); ++i){
        const QString &soureTableName = sourceTableNames.at(i);
        QModelIndex foundIndex = model->findByName(tablesIndex, soureTableName);
        Q_ASSERT(foundIndex.isValid());
        itemsToCompare.append(foundIndex);
        TableInfoForDataComparison info = tableOptions.value(soureTableName);
        targetTableNames.append(info.targetTableName.isEmpty() ? soureTableName : info.targetTableName);

        this->tableNamesToCompare = joinEnclosed(targetTableNames, ",", "'");
    }
}

void DataComparerHelper::sortTableNames(const QStringList &tableNames)
{
    emit statusChanged(tr("Determining compare order..."));

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

void DataComparerHelper::tableSortQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        subComparisonError("sort_referencing_tables_query", result.exception);
    }
}

void DataComparerHelper::tableSortRecordFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        subComparisonError("sort_referencing_tables_fetch", fetchResult.exception);
        return;
    }

    QString tables = fetchResult.oneRow.at(0);
    bool fullySorted = fetchResult.oneRow.at(1).toInt()==1;

    fillItemsToCompare(tables);

    this->needToDisableRefConstraints = (options->disableRefConstraints==DataComparisonOptions::Disable ||
                                         !fullySorted);
    if(needToDisableRefConstraints){
        disableRefConstraints();
    }else{
        startToCompare();
    }
}

void DataComparerHelper::tableSortFetchCompleted(const QString &)
{
}

void DataComparerHelper::loadTableColumns()
{
    if(currentItemIxToCompare >= itemsToCompare.size()){
        currentItemIxToCompare = 0;
        //fillItemsToCompare();
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

void DataComparerHelper::compareNextItem()
{
    if(currentItemIxToCompare >= itemsToCompare.size()){
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

        startComparerThread(tableItem->itemName());
    }
}

void DataComparerHelper::startComparerThread(const QString &tableName)
{
    Q_ASSERT(comparerThread==0);

    currentTableName = tableName;
    emit compareInfoAvailable(DataCompareInfo(tableName, tr("Starting")));

    comparerThread=new DataComparerThread(sourceSchema, sourceScheduler->getDb(),
                                          targetSchema, targetScheduler->getDb(),
                                          tableName, options, tableOptions.value(tableName),
                                          this);

    connect(comparerThread, SIGNAL(statusChanged(QString)), this, SIGNAL(statusChanged(QString)));
    connect(comparerThread, SIGNAL(compareInfoAvailable(DataCompareInfo)), this, SIGNAL(compareInfoAvailable(DataCompareInfo)));
    connect(comparerThread, SIGNAL(comparisonCompleted()), this, SLOT(tableComparisonCompleted()));
    connect(comparerThread, SIGNAL(compareError(QString,OciException)), this, SLOT(tableComparisonError(QString,OciException)));

    comparerThread->start();
}

void DataComparerHelper::subComparisonError(const QString &taskName, const OciException &exception)
{
    emit compareInfoAvailable(DataCompareInfo(currentTableName, tr("Error")));

    emit comparisonError(taskName, exception);

    this->deleteLater();
}

void DataComparerHelper::emitCompletedSignal()
{
    emit completed();

    this->deleteLater();
}

void DataComparerHelper::childrenPopulated(const QModelIndex &)
{
    loadTableColumns();
}

void DataComparerHelper::childrenPopulateError(const QModelIndex &, const OciException &exception)
{
    subComparisonError("populate_tree_child_nodes", exception);
}

void DataComparerHelper::tableComparisonCompleted()
{
    emit chunkCompleted(1);

    deleteComparerThread();
    compareNextItem();
}

void DataComparerHelper::tableComparisonError(const QString &taskName, const OciException &exception)
{
    deleteComparerThread();

    if(needToDisableRefConstraints &&
            options->comparisonMode==DataComparisonOptions::UpdateDatabase){
        lastExceptionTaskName=taskName;
        lastException=exception;
        enableRefConstraints();
    }else{
        subComparisonError(taskName, exception);
    }
}

void DataComparerHelper::deleteComparerThread()
{
    if(!comparerThread){
        return;
    }

    comparerThread->wait();
    delete comparerThread;
    comparerThread=0;
}

void DataComparerHelper::disableRefConstraints()
{
    emit statusChanged(tr("Disabling referential constraints"));

    QString disableQuery = QueryUtil::getQuery("disable_all_ref_constraints", targetScheduler->getDb());
    disableQuery.replace("{owner}", this->targetSchema);
    disableQuery.replace("{table_names}", this->tableNamesToCompare);

    if(options->comparisonMode==DataComparisonOptions::UpdateDatabase){
        targetScheduler->enqueueQuery(QString("$%1").arg(disableQuery),
                                      QList<Param*>(),
                                      this,
                                      "disable_all_ref_constraints",
                                      "refConstraintsDisabled");
    }else{
        emit compareInfoAvailable(DataCompareInfo("", "", 0,0,0, disableQuery));
        startToCompare();
    }
}

void DataComparerHelper::enableRefConstraints()
{
    emit statusChanged(tr("Enabling referential constraints"));

    enableRefContraintsQuery = QueryUtil::getQuery("enable_all_ref_constraints", targetScheduler->getDb());
    enableRefContraintsQuery.replace("{owner}", this->targetSchema);
    enableRefContraintsQuery.replace("{table_names}", this->tableNamesToCompare);

    if(options->comparisonMode==DataComparisonOptions::UpdateDatabase){
        targetScheduler->enqueueQuery(QString("$%1").arg(enableRefContraintsQuery),
                                  QList<Param*>(),
                                  this,
                                  "enable_all_ref_constraints",
                                  "refConstraintsEnabled");
    }else{
        emit compareInfoAvailable(DataCompareInfo("", "", 0,0,0, enableRefContraintsQuery));
        emitCompletedSignal();
    }
}

void DataComparerHelper::refConstraintsDisabled(const QueryResult &result)
{
    delete result.statement;

    if(result.hasError){
        subComparisonError(result.taskName, result.exception);
    }else{
        startToCompare();
    }
}

void DataComparerHelper::refConstraintsEnabled(const QueryResult &result)
{
    delete result.statement;

    if(result.hasError){
        OciException ex=result.exception;

        if(options->comparisonMode==DataComparisonOptions::UpdateDatabase){
            ex.addToErrorMessage(tr("Comparison completed successfully but application failed to enable referential constraints.\nThey need to be enabled manually with provided query.\n"));
        }

        if(!lastException.isEmpty()){
            lastException.addToErrorMessage(tr("Referential constraints must be enabled manually with provided query."), false);
        }

        emit compareInfoAvailable(DataCompareInfo("", "", 0,0,0, enableRefContraintsQuery));

        subComparisonError(lastExceptionTaskName.isEmpty() ? result.taskName : lastExceptionTaskName, lastException.isEmpty() ? ex : lastException);
    }else{
        if(lastExceptionTaskName.isEmpty()){
            emitCompletedSignal();
        }else{
            subComparisonError(lastExceptionTaskName, lastException);
        }
    }
}

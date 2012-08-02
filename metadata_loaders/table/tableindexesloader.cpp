#include "tableindexesloader.h"
#include "tableinfoloader.h"
#include "interfaces/iqueryscheduler.h"
#include "partitioninfoloader.h"

TableIndexesLoader::TableIndexesLoader(QObject *parent) :
    QObject(parent), subqueryCount(0)
{
    tableInfoLoader=static_cast<TableInfoLoader*>(parent);
    Q_ASSERT(tableInfoLoader);
}

void TableIndexesLoader::loadInfo()
{
    tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_indexes_for_editing", tableInfoLoader->getBasicQueryParams(), this,
                     "get_table_indexes_for_editing",
                     "queryCompleted",
                     "recordFetched",
                     "fetchCompleted");
}

void TableIndexesLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void TableIndexesLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    IndexInfo indexInfo=IndexInfo::fromFetchResult(result);
    indexes.append(indexInfo);

    subqueryCount++;
    tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_index_columns",
                     tableInfoLoader->getBasicQueryParams()
                     << new Param(":index_owner", indexInfo.owner)
                     << new Param(":index_name", indexInfo.name),
                     this,
                     QString("get_table_index_columns_%1").arg(indexes.size()),
                     "indexColumnsQueryCompleted",
                     "indexColumnsRecordFetched",
                     "", true);

    if(indexInfo.partitioning==2){
        subqueryCount++;
        PartitionInfoLoader *pInfoLoader=new PartitionInfoLoader(this);
        connect(pInfoLoader, SIGNAL(partitionInfoReady(TablePartitioningInfo,PartitionInfoLoader*)), this, SLOT(partitionInfoReady(TablePartitioningInfo,PartitionInfoLoader*)));
        connect(pInfoLoader, SIGNAL(loadError(QString,OciException,PartitionInfoLoader*)), this, SLOT(paritionInfoLoadError(QString,OciException,PartitionInfoLoader*)));
        pInfoLoader->loadPartitionInfo(tableInfoLoader->getQueryScheduler(), indexInfo.owner, indexInfo.name, true, indexes.size());

        indexInfo.partInfo.isEmpty=false;
    }else{
        indexInfo.partInfo.isEmpty=true;
    }
}

void TableIndexesLoader::fetchCompleted(const QString &/*taskName*/)
{
    if(subqueryCount==0){
        emit infoReady(indexes);
    }
}

void TableIndexesLoader::indexColumnsQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void TableIndexesLoader::indexColumnsRecordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    int rowId=result.taskName.mid(result.taskName.lastIndexOf('_')+1).toInt();
    Q_ASSERT(rowId>0 && rowId<=indexes.size());

    QString columns=result.oneRow.at(0);
    indexes[rowId-1].columns=columns;

    decreaseSubqueryCount();
}

void TableIndexesLoader::partitionInfoReady(const TablePartitioningInfo &pInfo, PartitionInfoLoader *loader)
{
    int indexId=loader->data;
    Q_ASSERT(indexId>0 && indexId<=indexes.size());

    int rowId=indexId-1;

    indexes[rowId].partInfo=pInfo;

    loader->deleteLater();

    decreaseSubqueryCount();
}

void TableIndexesLoader::paritionInfoLoadError(const QString &taskName, const OciException &ex, PartitionInfoLoader *loader)
{
    emit loadError(taskName, ex);

    loader->deleteLater();
}

void TableIndexesLoader::decreaseSubqueryCount()
{
    subqueryCount--;
    Q_ASSERT(subqueryCount>=0);
    if(subqueryCount==0){
        emit infoReady(indexes);
    }
}

#include "tablecolumninfoloader.h"
#include "tableinfoloader.h"
#include "interfaces/iqueryscheduler.h"

TableColumnInfoLoader::TableColumnInfoLoader(QObject *parent) :
    QObject(parent), lobSubqueryCount(0)
{
    tableInfoLoader=static_cast<TableInfoLoader*>(parent);
    Q_ASSERT(tableInfoLoader);
}

void TableColumnInfoLoader::loadInfo()
{
    tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_columns_for_editing",
                                             tableInfoLoader->getBasicQueryParams(),
                                             this,
                                             "get_table_columns_for_editing",
                                             "queryCompleted",
                                             "recordFetched",
                                             "fetchCompleted");
}

void TableColumnInfoLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void TableColumnInfoLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    ColumnInfo colInfo=ColumnInfo::fromFetchResult(result);
    columns.append(colInfo);

    if(colInfo.dataType.contains("LOB")){
        ++lobSubqueryCount;

        tableInfoLoader->getQueryScheduler()->enqueueQuery("get_lob_storage_info",
                         tableInfoLoader->getBasicQueryParams() << new Param(":column_name", colInfo.name),
                         this,
                         QString("get_lob_storage_info_col_%1").arg(colInfo.columnId),
                         "lobInfoQueryCompleted",
                         "lobInfoRecordFetched");

        colInfo.lobParams.isEmpty=false;
    }else{
        colInfo.lobParams.isEmpty=true;
    }
}

void TableColumnInfoLoader::fetchCompleted(const QString &/*taskName*/)
{
    if(lobSubqueryCount==0){
        emit infoReady(columns);
    }
}

void TableColumnInfoLoader::lobInfoQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        loadError(result.taskName, result.exception);
    }
}

void TableColumnInfoLoader::lobInfoRecordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    int columnId=result.taskName.mid(result.taskName.lastIndexOf('_')+1).toInt();
    Q_ASSERT(columnId>0);
    Q_ASSERT(columns.size()>=columnId);

    LobParams lobParams=LobParams::fromFetchResult(result);
    columns[columnId-1].lobParams=lobParams;

    --lobSubqueryCount;
    Q_ASSERT(lobSubqueryCount>=0);
    if(lobSubqueryCount==0){
        emit infoReady(columns);
    }
}

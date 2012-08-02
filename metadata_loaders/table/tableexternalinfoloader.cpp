#include "tableexternalinfoloader.h"
#include "tableinfoloader.h"
#include "interfaces/iqueryscheduler.h"

TableExternalInfoLoader::TableExternalInfoLoader(QObject *parent) :
    QObject(parent)
{
    tableInfoLoader=static_cast<TableInfoLoader*>(parent);
    Q_ASSERT(tableInfoLoader);
}

void TableExternalInfoLoader::loadInfo()
{
    tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_external_info_for_editing",
                     tableInfoLoader->getBasicQueryParams(),
                     this,
                     "get_table_external_info_for_editing",
                     "queryCompleted",
                     "recordFetched",
                     "fetchCompleted");
}

void TableExternalInfoLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void TableExternalInfoLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    info=TableExternalInfo::fromFetchResult(result);
}

void TableExternalInfoLoader::fetchCompleted(const QString &)
{
    tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_external_locations_for_editing",
                                             tableInfoLoader->getBasicQueryParams(),
                                             this,
                                             "get_table_external_locations_for_editing",
                                             "locationsQueryCompleted",
                                             "locationsRecordFetched",
                                             "locationsFetchCompleted");
}

void TableExternalInfoLoader::locationsQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void TableExternalInfoLoader::locationsRecordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    QString directoryName=result.colValue("DIRECTORY_NAME");
    QString location=result.colValue("LOCATION");
    info.locations.append(qMakePair(directoryName, location));
}

void TableExternalInfoLoader::locationsFetchCompleted(const QString &)
{
    emit infoReady(info);
}

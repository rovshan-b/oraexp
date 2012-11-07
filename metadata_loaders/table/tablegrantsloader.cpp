#include "tablegrantsloader.h"
#include "tableinfoloader.h"
#include "interfaces/iqueryscheduler.h"

TableGrantsLoader::TableGrantsLoader(QObject *parent) :
    QObject(parent)
{
    tableInfoLoader=static_cast<TableInfoLoader*>(parent);
    Q_ASSERT(tableInfoLoader);
}

void TableGrantsLoader::loadInfo()
{
    tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_grants_for_editing", tableInfoLoader->getBasicQueryParams(), this,
                     "get_table_grants_for_editing",
                     "queryCompleted",
                     "recordFetched",
                     "fetchCompleted");
}

void TableGrantsLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void TableGrantsLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    ObjectGrantInfo grantInfo=ObjectGrantInfo::fromFetchResult(result, DbTreeModel::Table);
    grants.append(grantInfo);
}

void TableGrantsLoader::fetchCompleted(const QString &)
{
    emit infoReady(grants);
}

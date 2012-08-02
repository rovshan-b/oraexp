#include "tabletriggersloader.h"
#include "tableinfoloader.h"
#include "interfaces/iqueryscheduler.h"

TableTriggersLoader::TableTriggersLoader(QObject *parent) :
    QObject(parent)
{
    tableInfoLoader=static_cast<TableInfoLoader*>(parent);
    Q_ASSERT(tableInfoLoader);
}

void TableTriggersLoader::loadInfo()
{
    tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_triggers_all_columns", tableInfoLoader->getBasicQueryParams(), this,
                     "get_table_triggers_for_detailed_view",
                     "queryCompleted",
                     "recordFetched",
                     "fetchCompleted");
}

void TableTriggersLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void TableTriggersLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    TriggerInfo triggerInfo=TriggerInfo::fromFetchResult(result);
    triggers.append(triggerInfo);
}

void TableTriggersLoader::fetchCompleted(const QString &)
{
    emit infoReady(triggers);
}

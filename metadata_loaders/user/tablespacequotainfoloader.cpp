#include "tablespacequotainfoloader.h"
#include "userinfoloader.h"

TablespaceQuotaInfoLoader::TablespaceQuotaInfoLoader(QObject *parent) :
    QObject(parent)
{
    userInfoLoader=static_cast<UserInfoLoader*>(parent);
    Q_ASSERT(userInfoLoader);
}

void TablespaceQuotaInfoLoader::loadInfo()
{
    Q_ASSERT(quotas.isEmpty());

    userInfoLoader->
            getQueryScheduler()->
            enqueueQuery("get_user_tablespace_quotas_as_dba",
                     QList<Param*>() << new Param(":owner", userInfoLoader->getObjectName()),
                     this,
                     "get_user_tablespace_quotas_as_dba",
                     "queryCompleted",
                     "recordFetched",
                     "fetchCompleted");
}

void TablespaceQuotaInfoLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void TablespaceQuotaInfoLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    TablespaceQuotaInfo quotaInfo=TablespaceQuotaInfo::fromFetchResult(result);
    quotas.append(quotaInfo);
}

void TablespaceQuotaInfoLoader::fetchCompleted(const QString &)
{
    emit infoReady(quotas);
}

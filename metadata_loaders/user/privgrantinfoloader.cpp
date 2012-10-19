#include "privgrantinfoloader.h"
#include "userinfoloader.h"

PrivGrantInfoLoader::PrivGrantInfoLoader(const QString &queryName, QObject *parent) :
    QObject(parent), queryName(queryName)
{
    userInfoLoader=static_cast<UserInfoLoader*>(parent);
    Q_ASSERT(userInfoLoader);
}

void PrivGrantInfoLoader::loadInfo()
{
    Q_ASSERT(grants.isEmpty());

    userInfoLoader->
            getQueryScheduler()->
            enqueueQuery(queryName,
                     QList<Param*>() << new Param(":owner", userInfoLoader->getObjectName()),
                     this,
                     queryName,
                     "queryCompleted",
                     "recordFetched",
                     "fetchCompleted");
}

void PrivGrantInfoLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void PrivGrantInfoLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    PrivGrantInfo grantInfo=PrivGrantInfo::fromFetchResult(result);
    grants.append(grantInfo);
}

void PrivGrantInfoLoader::fetchCompleted(const QString &)
{
    emit infoReady(grants);
}

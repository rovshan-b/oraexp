#include "objectgrantsloader.h"
#include "interfaces/iqueryscheduler.h"

ObjectGrantsLoader::ObjectGrantsLoader(DbTreeModel::DbTreeNodeType objectType, QObject *parent) :
    QObject(parent),
    objectType(objectType)
{
}

void ObjectGrantsLoader::loadInfo(IQueryScheduler *queryScheduler,
                                  const QString &queryName,
                                  const QList<Param*> &params)
{
    queryScheduler->enqueueQuery(queryName, params, this,
                                 queryName,
                                 "queryCompleted",
                                 "recordFetched",
                                 "fetchCompleted");
}

void ObjectGrantsLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void ObjectGrantsLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    ObjectGrantInfo grantInfo=ObjectGrantInfo::fromFetchResult(result, objectType);
    grants.append(grantInfo);
}

void ObjectGrantsLoader::fetchCompleted(const QString &)
{
    emit infoReady(grants);
}

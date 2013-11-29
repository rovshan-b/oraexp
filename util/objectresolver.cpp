#include "objectresolver.h"
#include "dbutil.h"

ObjectResolver::ObjectResolver(QObject *parent) :
    QObject(parent)
{
}

void ObjectResolver::resolveName(const QString &objectName, IQueryScheduler *queryScheduler)
{
    queryScheduler->enqueueQuery("resolve_name", QList<Param*>() << new Param("object_name", objectName),
                                 this, "resolve_name", "resolveQueryCompleted", "resolveResultReady", "resolveFetchCompleted",
                                 true);
}

void ObjectResolver::resolveQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit resolveError(result.exception, this);
    }
}

void ObjectResolver::resolveResultReady(const FetchResult &result)
{
    if(result.hasError){
        emit resolveError(result.exception, this);
    }else{
        QString objectTypeName = result.colValue("OBJECT_TYPE");

        emit resolved(objectTypeName,
                      DbUtil::getDbObjectNodeTypeByTypeName(objectTypeName),
                      result.colValue("SCHEMA_NAME"),
                      result.colValue("PART1"),
                      this);
    }
}

void ObjectResolver::resolveFetchCompleted(const QString &taskName)
{
}

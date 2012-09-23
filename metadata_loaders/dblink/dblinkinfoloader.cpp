#include "dblinkinfoloader.h"
#include "beans/dblinkinfo.h"
#include "interfaces/iqueryscheduler.h"

DbLinkInfoLoader::DbLinkInfoLoader(IQueryScheduler *queryScheduler, const QString &schemaName, const QString &objectName, QObject *parent) :
    MetadataLoader(queryScheduler, schemaName, objectName, parent)
{
}

DbLinkInfoLoader::~DbLinkInfoLoader()
{
    qDebug("deleted DbLinkInfoLoader");
}

void DbLinkInfoLoader::loadObjectInfo()
{
    queryScheduler->enqueueQuery("get_dblink",
                                 QList<Param*>() <<
                                 new Param("owner", schemaName) <<
                                 new Param("dblink_name", objectName),
                                 this,
                                 "get_dblink",
                                 "queryCompleted",
                                 "recordFetched",
                                 "fetchCompleted");
}


void DbLinkInfoLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception, this);
    }
}

void DbLinkInfoLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception, this);
    }else{
        DbLinkInfo *dbLinkInfo=new DbLinkInfo(DbLinkInfo::fromFetchResult(result));

        emit objectInfoReady(dbLinkInfo, this);
    }
}

void DbLinkInfoLoader::fetchCompleted(const QString &)
{

}

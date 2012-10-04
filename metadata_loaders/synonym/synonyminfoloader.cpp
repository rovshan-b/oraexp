#include "synonyminfoloader.h"
#include "beans/synonyminfo.h"
#include "interfaces/iqueryscheduler.h"

SynonymInfoLoader::SynonymInfoLoader(IQueryScheduler *queryScheduler, const QString &schemaName, const QString &objectName, QObject *parent) :
    MetadataLoader(queryScheduler, schemaName, objectName, parent)
{
}

SynonymInfoLoader::~SynonymInfoLoader()
{
    qDebug("deleted SynonymInfoLoader");
}

void SynonymInfoLoader::loadObjectInfo()
{
    queryScheduler->enqueueQuery("get_synonym",
                                 QList<Param*>() <<
                                 new Param("owner", schemaName) <<
                                 new Param("synonym_name", objectName),
                                 this,
                                 "get_synonym",
                                 "queryCompleted",
                                 "recordFetched",
                                 "fetchCompleted");
}


void SynonymInfoLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception, this);
    }
}

void SynonymInfoLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception, this);
    }else{
        SynonymInfo *synonymInfo=new SynonymInfo(SynonymInfo::fromFetchResult(result));

        emit objectInfoReady(synonymInfo, this);
    }
}

void SynonymInfoLoader::fetchCompleted(const QString &)
{

}

#include "sourceinfoloader.h"
#include "beans/sourceinfo.h"

SourceInfoLoader::SourceInfoLoader(IQueryScheduler *queryScheduler, const QString &schemaName,
                                   const QString &objectName,
                                   const QString &objectType,
                                   const QString &targetSchemaName,
                                   bool wrap,
                                   bool sqlTerminator,
                                   QObject *parent) :
    MetadataLoader(queryScheduler, schemaName, objectName, parent),
    objectType(objectType),
    targetSchemaName(targetSchemaName),
    wrap(wrap),
    sqlTerminator(sqlTerminator)
{
}

void SourceInfoLoader::loadObjectInfo()
{
    queryScheduler->enqueueQuery("get_object_source_code",
                                 QList<Param*>() <<
                                 new Param("object_type", objectType) <<
                                 new Param("object_name", objectName) <<
                                 new Param("owner", schemaName) <<
                                 new Param("target_owner", targetSchemaName) <<
                                 new Param("wrap", wrap) <<
                                 new Param("sql_terminator", sqlTerminator),
                                 this,
                                 "get_object_source_code",
                                 "queryCompleted",
                                 "recordFetched",
                                 "fetchCompleted",
                                 true);
}

void SourceInfoLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception, this);
    }
}

void SourceInfoLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception, this);
    }else{
        SourceInfo *sourceInfo=new SourceInfo();
        sourceInfo->owner=this->schemaName;
        sourceInfo->objectName=this->objectName;
        sourceInfo->objectType=this->objectType;
        sourceInfo->source=result.oneRow.at(0);

        emit objectInfoReady(sourceInfo, this);
    }
}

void SourceInfoLoader::fetchCompleted(const QString &)
{

}


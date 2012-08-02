#include "sequenceinfoloader.h"
#include "beans/sequenceinfo.h"
#include "interfaces/iqueryscheduler.h"

SequenceInfoLoader::SequenceInfoLoader(IQueryScheduler *queryScheduler, const QString &schemaName, const QString &objectName, QObject *parent) :
    MetadataLoader(queryScheduler, schemaName, objectName, parent)
{
}

SequenceInfoLoader::~SequenceInfoLoader()
{
    qDebug("deleted SequenceInfoLoader");
}

void SequenceInfoLoader::loadObjectInfo()
{
    queryScheduler->enqueueQuery("get_sequence",
                                 QList<Param*>() <<
                                 new Param("owner", schemaName) <<
                                 new Param("sequence_name", objectName),
                                 this,
                                 "get_sequence",
                                 "queryCompleted",
                                 "recordFetched",
                                 "fetchCompleted");
}


void SequenceInfoLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception, this);
    }
}

void SequenceInfoLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception, this);
    }else{
        SequenceInfo *sequenceInfo=new SequenceInfo(SequenceInfo::fromFetchResult(result));

        emit objectInfoReady(sequenceInfo, this);
    }
}

void SequenceInfoLoader::fetchCompleted(const QString &)
{

}

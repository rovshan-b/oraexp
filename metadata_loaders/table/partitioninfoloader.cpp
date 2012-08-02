#include "partitioninfoloader.h"
#include "util/param.h"
#include "connectivity/dbconnection.h"
#include "interfaces/iqueryscheduler.h"
#include <QDebug>

PartitionInfoLoader::PartitionInfoLoader(QObject *parent) :
    QObject(parent)
{
}

PartitionInfoLoader::~PartitionInfoLoader()
{
    qDebug("deleted PartitionInfoLoader");
}

void PartitionInfoLoader::loadPartitionInfo(IQueryScheduler *queryScheduler, const QString &schemaName, const QString &objectName, bool isIndex, int data)
{
    isPartitioned=false;
    this->queryScheduler=queryScheduler;
    this->schemaName=schemaName;
    this->objectName=objectName;
    this->isIndex=isIndex;
    this->data=data;

    QString queryName=isIndex ? "get_index_partitioning_info" : "get_table_partitioning_info";
    queryScheduler->enqueueQuery(queryName,
                     QList<Param*>()
                     << new Param(":owner", schemaName)
                     << new Param(isIndex ? ":index_name" : ":table_name", objectName),
                     this,
                     queryName,
                     "partitioningInfoLoaded",
                     "partitionInfoFetched",
                     "partitionInfoFetchCompleted");
}

void PartitionInfoLoader::partitioningInfoLoaded(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception, this);
    }
}

void PartitionInfoLoader::partitionInfoFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        emit loadError(fetchResult.taskName, fetchResult.exception, this);
        return;
    }

    isPartitioned=true;
    QString pType=fetchResult.colValue("PARTITIONING_TYPE");
    QString subpType=fetchResult.colValue("SUBPARTITIONING_TYPE");
    bool complexPartitioning=(subpType!="NONE");

    partInfo.partitionInfo.partitioningStrategy=complexPartitioning ? 1 : 0;

    OraExp::PartitionType partitionType=partitionTypeFromName(pType);
    if(partitionType==OraExp::PartitionTypeUnknown){
        emit loadError("partitioning_type_not_supported",
                       OciException(tr("%1 partitioning type is not supported by application. Partitions tab will be disabled.").arg(pType),
                                                                       0, 0, 0), this);
        return;
    }

    partInfo.partitionInfo.type=partitionType;
    partInfo.partitionInfo.columns=fetchResult.colValue("PARTITION_KEY_COLUMNS");
    QString interval = fetchResult.colValue("INTERVAL");
    if(!interval.isEmpty()){
        partInfo.partitionInfo.interval=interval;
    }

    if(complexPartitioning){
        partInfo.subpartitionInfo.type=partitionTypeFromName(subpType);
        partInfo.subpartitionInfo.columns=fetchResult.colValue("SUBPARTITION_KEY_COLUMNS");

        partInfo.subpartitionTemplateInfo.type=partInfo.subpartitionInfo.type;
    }

    const QString listQueryCompletedSlotName="partitionListQueryCompleted";
    const QString listRecordFetchedSlotName="partitionListRecordFetched";
    const QString listFetchCompletedSlotName="partitionListFetchCompleted";

    QString queryName=isIndex ? "get_index_partitions_for_editing" : "get_table_partitions_for_editing";
    queryScheduler->enqueueQuery(queryName,
                     getQueryParams(schemaName, objectName, isIndex),
                     this,
                     queryName,
                     listQueryCompletedSlotName,
                     listRecordFetchedSlotName,
                     listFetchCompletedSlotName);

    if(complexPartitioning){
        queryScheduler->enqueueQuery("get_table_subpartitions_for_editing",
                         getQueryParams(schemaName, objectName, isIndex),
                         this,
                         "get_table_subpartitions_for_editing",
                         listQueryCompletedSlotName,
                         listRecordFetchedSlotName,
                         listFetchCompletedSlotName);

        queryScheduler->enqueueQuery("get_table_subpartition_template_for_editing",
                         getQueryParams(schemaName, objectName, isIndex),
                         this,
                         "get_table_subpartition_template_for_editing",
                         listQueryCompletedSlotName,
                         listRecordFetchedSlotName,
                         listFetchCompletedSlotName);
    }

    /*partitionsTab->loadFromPartitioningInfo(pInfo, true);
    subpartitionsTab->loadFromPartitioningInfo(subpInfo, true);

    partitionsTab->setEditMode();
    subpartitionsTab->setEditMode();
    subpartitionTemplateTab->setEditMode();*/
}

QList<Param*> PartitionInfoLoader::getQueryParams(const QString &schemaName, const QString &objectName, bool isIndex)
{
    return QList<Param*>()
            << new Param(":owner", schemaName)
            << new Param(isIndex ? ":index_name" : ":table_name", objectName);
}

void PartitionInfoLoader::partitionInfoFetchCompleted(const QString &)
{
    if(!isPartitioned){
        partInfo.isEmpty=true;
        emit partitionInfoReady(partInfo, this);
    }
}

void PartitionInfoLoader::partitionListQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception, this);
    }
}

void PartitionInfoLoader::partitionListRecordFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        emit loadError(fetchResult.taskName, fetchResult.exception, this);
        return;
    }

    PartitionInfo partitionInfo=PartitionInfo::fromFetchResult(fetchResult);

    if(fetchResult.taskName=="get_table_partitions_for_editing" ||
            fetchResult.taskName=="get_index_partitions_for_editing"){
        partInfo.partitionInfo.partitions.append(partitionInfo);
    }else if(fetchResult.taskName=="get_table_subpartitions_for_editing"){
        partInfo.subpartitionInfo.partitions.append(partitionInfo);
    }else if(fetchResult.taskName=="get_table_subpartition_template_for_editing"){
        partInfo.subpartitionTemplateInfo.partitions.append(partitionInfo);
    }
}

void PartitionInfoLoader::partitionListFetchCompleted(const QString &taskName)
{
    bool complexPartitioning=partInfo.partitionInfo.partitioningStrategy==1;
    if(!complexPartitioning ||
            (complexPartitioning && taskName=="get_table_subpartition_template_for_editing")){
        partInfo.isEmpty=false;
        emit partitionInfoReady(partInfo, this);
    }
}

OraExp::PartitionType PartitionInfoLoader::partitionTypeFromName(const QString &partitionType) const
{
    OraExp::PartitionType result=OraExp::PartitionTypeUnknown;

    if(partitionType=="RANGE"){
        result=OraExp::PartitionTypeRange;
    }else if(partitionType=="LIST"){
        result=OraExp::PartitionTypeList;
    }else if(partitionType=="HASH"){
        result=OraExp::PartitionTypeHash;
    }else if(partitionType=="REFERENCE"){
        result=OraExp::PartitionTypeReference;
    }

    return result;
}

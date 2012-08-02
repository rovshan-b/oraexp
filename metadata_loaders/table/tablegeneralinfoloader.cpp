#include "tablegeneralinfoloader.h"
#include "tableinfoloader.h"
#include "interfaces/iqueryscheduler.h"
#include "util/dbutil.h"

#define QUERY_COMPLETED_SLOT_NAME "queryCompleted"
#define FETCH_SLOT_NAME "recordFetched"
#define FETCH_COMPLETED_SLOT_NAME "fetchCompleted"

TableGeneralInfoLoader::TableGeneralInfoLoader(QObject *parent) :
    QObject(parent), subqueryCount(0)
{
    tableInfoLoader=static_cast<TableInfoLoader*>(parent);
    Q_ASSERT(tableInfoLoader);
}

void TableGeneralInfoLoader::loadInfo()
{
    qDebug("loading TableGeneralInfo");

    subqueryCount=0;

    QList<Param*> params=tableInfoLoader->getBasicQueryParams();

    tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_details_for_detailed_view", params, this, "get_table_details",
                                 QUERY_COMPLETED_SLOT_NAME, FETCH_SLOT_NAME, FETCH_COMPLETED_SLOT_NAME);

}

void TableGeneralInfoLoader::queryCompleted(const QueryResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
    }
}

void TableGeneralInfoLoader::recordFetched(const FetchResult &result)
{
    if(result.hasError){
        emit loadError(result.taskName, result.exception);
        return;
    }

    if(result.taskName=="get_table_details"){
        populateTableDetails(result);
    }else if(result.taskName=="get_table_comments"){
        --subqueryCount;
        generalInfo.comments=result.oneRow.at(0);
    }else if(result.taskName=="get_table_iot_details"){
        --subqueryCount;
        populateIOTDetails(result);
    }else if(result.taskName=="get_iot_overflow_table_details"){
        --subqueryCount;
        generalInfo.indexOrganizedProperties.storageParams = StorageParams::fromFetchResult(result);
    }

    Q_ASSERT(subqueryCount>=0);
    if(subqueryCount==0){
        qDebug("TableGeneralInfo loaded");

        emit infoReady(generalInfo);
    }
}

void TableGeneralInfoLoader::fetchCompleted(const QString &/*taskName*/)
{

}

void TableGeneralInfoLoader::populateTableDetails(const FetchResult &result)
{
    generalInfo.schema=tableInfoLoader->getSchemaName();
    generalInfo.tableName=tableInfoLoader->getTableName();

    generalInfo.tableType=DbUtil::getTableType(
                    result.colValue("TEMPORARY"),
                    result.colValue("DURATION"),
                    result.colValue("IOT_TYPE"),
                    result.colValue("IS_EXTERNAL")
                );

    generalInfo.storageParams = StorageParams::fromFetchResult(result);
    generalInfo.additionalAttributes = TableAdditionalAttributes::fromFetchResult(result);

    QList<Param*> params=tableInfoLoader->getBasicQueryParams();

    ++subqueryCount;
    tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_comments", params, this, "get_table_comments",
                                 QUERY_COMPLETED_SLOT_NAME, FETCH_SLOT_NAME, FETCH_COMPLETED_SLOT_NAME);

    if(generalInfo.tableType==OraExp::TableTypeIndexOrganized){
        ++subqueryCount;
        tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_iot_details", params, this, "get_table_iot_details",
                         QUERY_COMPLETED_SLOT_NAME, FETCH_SLOT_NAME, FETCH_COMPLETED_SLOT_NAME);
    }
}

void TableGeneralInfoLoader::populateIOTDetails(const FetchResult &result)
{
    generalInfo.indexOrganizedProperties = IndexOrganizedTableProperties::fromFetchResult(result);

    if(generalInfo.indexOrganizedProperties.overflow){
        ++subqueryCount;
        QList<Param*> params=tableInfoLoader->getBasicQueryParams();
        params[1]->setStrValue(result.colValue("OVERFLOW_TABLE_NAME"));
        tableInfoLoader->getQueryScheduler()->enqueueQuery("get_table_details_for_detailed_view", params, this, "get_iot_overflow_table_details",
                         QUERY_COMPLETED_SLOT_NAME, FETCH_SLOT_NAME, FETCH_COMPLETED_SLOT_NAME);
    }
}

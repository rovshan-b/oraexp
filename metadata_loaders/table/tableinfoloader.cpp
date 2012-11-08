#include "tableinfoloader.h"
#include "interfaces/iqueryscheduler.h"
#include "util/dbutil.h"
#include <QDebug>

TableInfoLoader::TableInfoLoader(IQueryScheduler *queryScheduler,
                                 const QString &schemaName,
                                 const QString &tableName,
                                 QObject *parent,
                                 bool loadTriggerInfo) :
    MetadataLoader(queryScheduler, schemaName, tableName, parent),
    tableInfo(0),
    loadTriggerInfo(loadTriggerInfo),
    generalInfoLoader(this),
    columnListLoader(this),
    constraintListLoader(this),
    indexListLoader(this),
    partitionInfoLoader(this),
    externalInfoLoader(this),
    triggerListLoader(this),
    grantListLoader(this)
{
    connect(&generalInfoLoader, SIGNAL(infoReady(TableGeneralInfo)), this, SLOT(tableGeneralInfoReady(TableGeneralInfo)));
    connect(&generalInfoLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));

    connect(&columnListLoader, SIGNAL(infoReady(QList<ColumnInfo>)), this, SLOT(tableColumnInfoReady(QList<ColumnInfo>)));
    connect(&columnListLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));

    connect(&constraintListLoader, SIGNAL(infoReady(QList<ConstraintInfo>, bool)), this, SLOT(tableConstraintInfoReady(QList<ConstraintInfo>, bool)));
    connect(&constraintListLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));

    connect(&indexListLoader, SIGNAL(infoReady(QList<IndexInfo>)), this, SLOT(tableIndexInfoReady(QList<IndexInfo>)));
    connect(&indexListLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));

    connect(&partitionInfoLoader, SIGNAL(partitionInfoReady(TablePartitioningInfo,PartitionInfoLoader*)), this, SLOT(partitionInfoReady(TablePartitioningInfo,PartitionInfoLoader*)));
    connect(&partitionInfoLoader, SIGNAL(loadError(QString,OciException,PartitionInfoLoader*)), this, SLOT(paritionInfoLoadError(QString,OciException,PartitionInfoLoader*)));

    connect(&externalInfoLoader, SIGNAL(infoReady(TableExternalInfo)), this, SLOT(tableExternalInfoReady(TableExternalInfo)));
    connect(&externalInfoLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));

    connect(&triggerListLoader, SIGNAL(infoReady(QList<TriggerInfo>)), this, SLOT(tableTriggerInfoReady(QList<TriggerInfo>)));
    connect(&triggerListLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));

    connect(&grantListLoader, SIGNAL(infoReady(QList<ObjectGrantInfo>)), this, SLOT(tableGrantInfoReady(QList<ObjectGrantInfo>)));
    connect(&grantListLoader, SIGNAL(loadError(QString,OciException)), this, SLOT(subInfoLoadError(QString,OciException)));
}

TableInfoLoader::~TableInfoLoader()
{
    qDebug("deleted TableInfoLoader");
}

QList<Param*> TableInfoLoader::getBasicQueryParams() const
{
    QList<Param*> params;

    params.append(new Param(":owner", schemaName));
    params.append(new Param(":table_name", objectName));

    return params;
}

void TableInfoLoader::loadObjectInfo()
{
    Q_ASSERT(tableInfo==0);
    tableInfo=new TableInfo();

    partsToLoad.enqueue(TableInfoPartGeneralInfo);
    partsToLoad.enqueue(TableInfoPartColumnInfo);
    partsToLoad.enqueue(TableInfoPartExternalInfo);
    partsToLoad.enqueue(TableInfoPartGrantInfo);

    loadTablePartInfo();
}

void TableInfoLoader::loadTablePartInfo()
{
    if(partsToLoad.isEmpty()){
        emitReadySignal();
        return;
    }

    TableInfoPart part = partsToLoad.dequeue();
    switch(part){
    case TableInfoPartGeneralInfo:
        generalInfoLoader.loadInfo();
        break;
    case TableInfoPartColumnInfo:
        columnListLoader.loadInfo();
        break;
    case TableInfoPartExternalInfo:
        if(tableInfo->generalInfo.tableType==OraExp::TableTypeExternal){
            externalInfoLoader.loadInfo();
        }else{
            partsToLoad.enqueue(TableInfoPartConstraintInfo);
            partsToLoad.enqueue(TableInfoPartCheckConstraintInfo);
            partsToLoad.enqueue(TableInfoPartIndexInfo);
            partsToLoad.enqueue(TableInfoPartPartitionInfo);
            if(loadTriggerInfo){
                partsToLoad.enqueue(TableInfoPartTriggerInfo);
            }

            loadTablePartInfo();
        }
        break;
    case TableInfoPartConstraintInfo:
        constraintListLoader.loadInfo(false);
        break;
    case TableInfoPartCheckConstraintInfo:
        constraintListLoader.loadInfo(true);
        break;
    case TableInfoPartIndexInfo:
        indexListLoader.loadInfo();
        break;
    case TableInfoPartPartitionInfo:
        partitionInfoLoader.loadPartitionInfo(this->queryScheduler, this->schemaName, this->objectName, false);
        break;
    case TableInfoPartTriggerInfo:
        triggerListLoader.loadInfo();
        break;
    case TableInfoPartGrantInfo:
        grantListLoader.loadInfo(this->queryScheduler, "get_table_grants_for_editing", getBasicQueryParams());
        break;
    default:
        qDebug("Unsupported TableInfoPart");
        break;
    }
}

void TableInfoLoader::tableGeneralInfoReady(const TableGeneralInfo &tableGeneralInfo)
{
    qDebug("table general info loaded");
    tableInfo->generalInfo=tableGeneralInfo;

    loadTablePartInfo();
}

void TableInfoLoader::tableColumnInfoReady(const QList<ColumnInfo> &tableColumns)
{
    qDebug("table column info loaded");
    tableInfo->columns=tableColumns;

    loadTablePartInfo();
}

void TableInfoLoader::tableConstraintInfoReady(const QList<ConstraintInfo> &tableConstraints, bool checkConstraints)
{
    qDebug() << "table constraint info loaded, checkConstraints=" << checkConstraints;
    if(checkConstraints){
        tableInfo->checkConstraints=tableConstraints;
    }else{
        tableInfo->constraints=tableConstraints;
    }

    loadTablePartInfo();
}

void TableInfoLoader::tableIndexInfoReady(const QList<IndexInfo> &tableIndexes)
{
    qDebug("table index info loaded");
    tableInfo->indexes=tableIndexes;

    loadTablePartInfo();
}

void TableInfoLoader::partitionInfoReady(const TablePartitioningInfo &pInfo, PartitionInfoLoader *)
{
    qDebug("table partition info loaded");
    tableInfo->partitioningInfo=pInfo; //done

    //loader->deleteLater(); will be deleted by class destructor

    loadTablePartInfo();
}

void TableInfoLoader::paritionInfoLoadError(const QString &taskName, const OciException &ex, PartitionInfoLoader *)
{
    emit loadError(taskName, ex, this);

    //loader->deleteLater(); will be deleted by class destructor
}

void TableInfoLoader::tableExternalInfoReady(const TableExternalInfo &tableExternalInfo)
{
    qDebug("table external info loaded");
    tableInfo->externalInfo=tableExternalInfo; //done

    loadTablePartInfo();
}

void TableInfoLoader::tableTriggerInfoReady(const QList<TriggerInfo> &tableTriggers)
{
    qDebug("table trigger info loader");
    tableInfo->triggers=tableTriggers;

    loadTablePartInfo();
}

void TableInfoLoader::tableGrantInfoReady(const QList<ObjectGrantInfo> &tableGrants)
{
    qDebug("table grant info loaded");
    tableInfo->grants=tableGrants;

    loadTablePartInfo();
}

void TableInfoLoader::emitReadySignal()
{
    tableInfo->removeGeneratedNotNullConstraints();

    emit objectInfoReady(tableInfo, this);
}

void TableInfoLoader::subInfoLoadError(const QString &taskName, const OciException &ex)
{
    delete tableInfo;

    emit loadError(taskName, ex, this);
}

#include "dbobjectcomparehelper.h"
#include "dbobjectscomparehelper.h"
#include "metadata_loaders/metadataloaderfactory.h"
#include "metadata_loaders/metadataloader.h"
#include "util/dbutil.h"

DbObjectCompareHelper::DbObjectCompareHelper(DbTreeModel::DbTreeNodeType objectType,
                                             const QString &compareResult,
                                             const QString &currentObjectName,
                                             QObject *parent) :
    QObject(parent), compareResult(compareResult), currentObjectName(currentObjectName), needsTargetObject(true), objectType(objectType),
    sourceObjectInfo(0)
{
    dbObjectsCompareHelper=qobject_cast<DbObjectsCompareHelper*>(parent);
    Q_ASSERT(dbObjectsCompareHelper);
}

DbObjectCompareHelper::~DbObjectCompareHelper()
{
    delete sourceObjectInfo;
}

void DbObjectCompareHelper::generateDdl()
{
    MetadataLoader *sourceInfoLoader=MetadataLoaderFactory::createMetadataLoader(objectType,
                                                                                 dbObjectsCompareHelper->getSourceQueryScheduler(),
                                                                                 dbObjectsCompareHelper->getSourceSchemaName(),
                                                                                 currentObjectName,
                                                                                 dbObjectsCompareHelper->getTargetSchemaName(),
                                                                                 dbObjectsCompareHelper->compareOptions()->sourceCodeOptions.wrap,
                                                                                 this);
    Q_ASSERT(sourceInfoLoader);

    connect(sourceInfoLoader, SIGNAL(objectInfoReady(DbObjectInfo*,MetadataLoader*)), this, SLOT(sourceObjectInfoReady(DbObjectInfo*,MetadataLoader*)));
    connect(sourceInfoLoader, SIGNAL(loadError(QString,OciException,MetadataLoader*)), this, SLOT(loadError(QString,OciException,MetadataLoader*)));
    sourceInfoLoader->loadObjectInfo();
}

void DbObjectCompareHelper::sourceObjectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader)
{
    Q_ASSERT(sourceObjectInfo==0);
    sourceObjectInfo = objectInfo;

    emit changedObjectDetected(currentObjectName, objectType, compareResult);

    if(compareResult=="new" || !needsTargetObject){
        QPair<QString,QString> ddlPair = generateCreateDdl(sourceObjectInfo, dbObjectsCompareHelper->compareOptions());

        emit compareInfoReady(ddlPair.first, ddlPair.second, this);
    }else{
        MetadataLoader *targetInfoLoader=MetadataLoaderFactory::createMetadataLoader(objectType,
                                                                                     dbObjectsCompareHelper->getTargetQueryScheduler(),
                                                                                     dbObjectsCompareHelper->getTargetSchemaName(),
                                                                                     currentObjectName,
                                                                                     dbObjectsCompareHelper->getTargetSchemaName(),
                                                                                     dbObjectsCompareHelper->compareOptions()->sourceCodeOptions.wrap,
                                                                                     this);

        Q_ASSERT(targetInfoLoader);
        connect(targetInfoLoader, SIGNAL(objectInfoReady(DbObjectInfo*,MetadataLoader*)), this, SLOT(targetObjectInfoReady(DbObjectInfo*,MetadataLoader*)));
        connect(targetInfoLoader, SIGNAL(loadError(QString,OciException,MetadataLoader*)), this, SLOT(loadError(QString,OciException,MetadataLoader*)));
        targetInfoLoader->loadObjectInfo();
    }

    loader->deleteLater();
}

void DbObjectCompareHelper::targetObjectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader)
{
    QPair<QString,QString> ddlPair = generateDiffDdl(sourceObjectInfo, objectInfo, dbObjectsCompareHelper->compareOptions());
    emit compareInfoReady(ddlPair.first, ddlPair.second, this);

    delete objectInfo;
    loader->deleteLater();
}

void DbObjectCompareHelper::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    emit compareError(taskName, ex, this);

    loader->deleteLater();
}

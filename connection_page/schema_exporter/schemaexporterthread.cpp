#include "schemaexporterthread.h"
#include "metadata_loaders/table/tableinfoloader.h"
#include "metadata_loaders/code/sourceinfoloader.h"
#include "beans/sourceinfo.h"
#include "beans/schemaexportoptions.h"
#include "code_generators/ddl/tableddlgenerator.h"
#include "util/dbutil.h"
#include <QDebug>

SchemaExporterThread::SchemaExporterThread(DbTreeModel::DbTreeNodeType parentNodeType,
                                           const QString &schemaName,
                                           const QStringList &objectNames,
                                           SchemaExportOptions *options,
                                           IQueryScheduler *queryScheduler, QObject *parent) :
    StopableThread(parent),
    parentNodeType(parentNodeType),
    schemaName(schemaName),
    objectNames(objectNames),
    options(options),
    queryScheduler(queryScheduler)
{
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void SchemaExporterThread::run()
{
    loadNextObject();

    exec();
}

void SchemaExporterThread::loadNextObject()
{
    if(objectNames.isEmpty() || this->stopped){
        emit exportCompleted();
        exit();
        return;
    }

    currentObjectName = objectNames.takeFirst();

    emit statusChanged(tr("Exporting %1").arg(currentObjectName));

    MetadataLoader *metadataLoader;

    switch(parentNodeType){
    case DbTreeModel::Tables:
        metadataLoader = new TableInfoLoader(queryScheduler, schemaName, currentObjectName, 0, true);
        break;
    default:
        metadataLoader = new SourceInfoLoader(queryScheduler, schemaName, currentObjectName,
                                          DbUtil::getDbObjectTypeNameByNodeType(parentNodeType),
                                          schemaName, false, true, 0);
        break;
    }

    Q_ASSERT(metadataLoader);
    connect(metadataLoader, SIGNAL(objectInfoReady(DbObjectInfo*,MetadataLoader*)), this, SLOT(objectInfoReady(DbObjectInfo*,MetadataLoader*)));
    connect(metadataLoader, SIGNAL(loadError(QString,OciException,MetadataLoader*)), this, SLOT(loadError(QString,OciException,MetadataLoader*)));
    metadataLoader->loadObjectInfo();
}

void SchemaExporterThread::objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader)
{
    QString ddl;

    if(parentNodeType==DbTreeModel::Tables){
        TableInfo *tableInfo = static_cast<TableInfo*>(objectInfo);
        tableInfo->prepareForOptions(options->tableOptions.newObjectStorageOptions);
        ddl = TableDdlGenerator::generateDdl(*tableInfo, true, true, QList<OraExp::ConstraintType>(),
                                             options->tableOptions.grants,
                                             options->tableOptions);
    }else{
        SourceInfo *sourceInfo = static_cast<SourceInfo*>(objectInfo);
        ddl = sourceInfo->source;
    }

    qDebug() << ddl;

    delete objectInfo;
    delete loader;

    emit objectExported(parentNodeType, currentObjectName);

    loadNextObject();
}

void SchemaExporterThread::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    delete loader;

    emit exportError(taskName, ex);

    exit();
}

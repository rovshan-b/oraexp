#include "schemaexporterworker.h"
#include "metadata_loaders/table/tableinfoloader.h"
#include "metadata_loaders/code/sourceinfoloader.h"
#include "metadata_loaders/sequence/sequenceinfoloader.h"
#include "beans/sourceinfo.h"
#include "beans/sequenceinfo.h"
#include "beans/schemaexportoptions.h"
#include "code_generators/ddl/tableddlgenerator.h"
#include "util/dbutil.h"
#include "util/filesystemutil.h"
#include <QDebug>

SchemaExporterWorker::SchemaExporterWorker(DbTreeModel::DbTreeNodeType parentNodeType,
                                           const QString &schemaName,
                                           const QStringList &objectNames,
                                           SchemaExportOptions *options,
                                           QIODevice::OpenMode fileOpenMode,
                                           IQueryScheduler *queryScheduler,
                                           QObject *parent) :
    QObject(parent),
    parentNodeType(parentNodeType),
    schemaName(schemaName),
    objectNames(objectNames),
    options(options),
    fileOpenMode(fileOpenMode),
    queryScheduler(queryScheduler),
    stopped(false),
    textStream(0),
    file(0)
{

}

SchemaExporterWorker::~SchemaExporterWorker()
{
    cleanup();
}

void SchemaExporterWorker::cleanup()
{
    delete textStream;
    delete file;
}

void SchemaExporterWorker::start()
{
    loadNextObject();
}

void SchemaExporterWorker::stop()
{
    this->stopped=true;
}

void SchemaExporterWorker::loadNextObject()
{
    if(objectNames.isEmpty() || this->stopped){

        if(textStream){
            if(!endDdl.isEmpty()){
                *textStream << endDdl;
            }
            textStream->flush();
        }

        emit exportCompleted();
        return;
    }

    if(textStream==0){
        QString errorMessage;
        if(!FileSystemUtil::createTextStream(options->filename,
                                            options->encoding,
                                            options->bom,
                                            this->fileOpenMode | QIODevice::Text,
                                            &textStream,
                                            &file,
                                            &errorMessage)){
            emit exportError("open_file_for_writing", OciException(errorMessage));
            return;
        }
    }

    currentObjectName = objectNames.takeFirst();

    emit statusChanged(tr("Exporting %1").arg(currentObjectName));
    emit objectExportStarted(parentNodeType, currentObjectName);

    MetadataLoader *metadataLoader;

    switch(parentNodeType){
    case DbTreeModel::Tables:
        metadataLoader = new TableInfoLoader(queryScheduler, schemaName, currentObjectName, 0, true);
        break;
    case DbTreeModel::Sequences:
        metadataLoader = new SequenceInfoLoader(queryScheduler, schemaName, currentObjectName, 0);
        break;
    default:
        metadataLoader = new SourceInfoLoader(queryScheduler, schemaName, currentObjectName,
                                          DbUtil::getDbObjectTypeNameByNodeType(parentNodeType),
                                          schemaName, options->sourceCodeOptions.wrap, true, 0);
        break;
    }

    Q_ASSERT(metadataLoader);
    connect(metadataLoader, SIGNAL(objectInfoReady(DbObjectInfo*,MetadataLoader*)), this, SLOT(objectInfoReady(DbObjectInfo*,MetadataLoader*)));
    connect(metadataLoader, SIGNAL(loadError(QString,OciException,MetadataLoader*)), this, SLOT(loadError(QString,OciException,MetadataLoader*)));
    metadataLoader->loadObjectInfo();
}

void SchemaExporterWorker::objectInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader)
{
    QString ddl;

    if(parentNodeType==DbTreeModel::Tables){
        TableInfo *tableInfo = static_cast<TableInfo*>(objectInfo);
        tableInfo->prepareForOptions(options->tableOptions.newObjectStorageOptions);
        QPair<QString,QString> ddlPair = TableDdlGenerator::generateDdlForExporting(*tableInfo, options->tableOptions);
        ddl = ddlPair.first;

        if(!ddlPair.second.isEmpty()){
            endDdl.append(ddlPair.second).append("\n");
        }
    }else if(parentNodeType==DbTreeModel::Sequences){
        SequenceInfo *sequenceInfo = static_cast<SequenceInfo*>(objectInfo);
        ddl = sequenceInfo->generateDdl(options->sequenceOptions);
        ddl = QString("%1;").arg(ddl);
    }else{
        SourceInfo *sourceInfo = static_cast<SourceInfo*>(objectInfo);
        ddl = sourceInfo->source;
    }

    *textStream << ddl << "\n";

    delete objectInfo;
    loader->deleteLater();

    emit objectExported(parentNodeType, currentObjectName);

    loadNextObject();
}

void SchemaExporterWorker::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    loader->deleteLater();

    emit exportError(taskName, ex);
}

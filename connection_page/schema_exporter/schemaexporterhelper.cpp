#include "schemaexporterhelper.h"
#include "beans/schemaexportoptions.h"
#include "schemaexporterworker.h"
#include <QDebug>

SchemaExporterHelper::SchemaExporterHelper(const QString &schemaName,
                                           IQueryScheduler *queryScheduler,
                                           DbTreeModel *model,
                                           SchemaExportOptions *options,
                                           QObject *parent) :
    SchemaOperationHelper(schemaName, queryScheduler, model, parent),
    options(options),
    justStarted(true)
{
}

SchemaExporterHelper::~SchemaExporterHelper()
{
    delete options;
    deleteWorker();
}

void SchemaExporterHelper::stop()
{
    SchemaOperationHelper::stop();

    if(worker){
        worker->stop();
    }
}

void SchemaExporterHelper::startComparer(DbTreeModel::DbTreeNodeType parentNodeType, const QStringList &checkedChildNames)
{
    worker = new SchemaExporterWorker(parentNodeType,
                                      sourceSchema,
                                      checkedChildNames,
                                      options,
                                      justStarted ? QIODevice::WriteOnly : QIODevice::Append,
                                      sourceScheduler,
                                      0);

    if(this->stopped){
        worker->stop();
    }

    connect(worker, SIGNAL(statusChanged(QString)), this, SIGNAL(statusChanged(QString)));
    connect(worker, SIGNAL(exportCompleted()), this, SLOT(exportCompleted()));
    connect(worker, SIGNAL(objectExportStarted(DbTreeModel::DbTreeNodeType,QString)), this, SIGNAL(objectExportStarted(DbTreeModel::DbTreeNodeType,QString)));
    connect(worker, SIGNAL(objectExported(DbTreeModel::DbTreeNodeType,QString)), this, SLOT(objectExported(DbTreeModel::DbTreeNodeType,QString)));
    connect(worker, SIGNAL(exportError(QString,OciException)), this, SLOT(subComparisonError(QString,OciException)));

    worker->start();
}

void SchemaExporterHelper::objectExported(DbTreeModel::DbTreeNodeType parentNodeType, const QString &objectName)
{
    if(justStarted){
        justStarted=false;
    }

    emit chunkCompleted(1);

    emit objectExportCompleted(parentNodeType, objectName);
}

void SchemaExporterHelper::exportCompleted()
{
    deleteWorker();
    compareNextParentItem();
}

void SchemaExporterHelper::deleteWorker()
{
    if(worker==0){
        return;
    }

    worker->deleteLater();
    worker = 0;
}

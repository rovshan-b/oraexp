#include "schemaexporterhelper.h"
#include "beans/schemaexportoptions.h"
#include "schemaexporterthread.h"

SchemaExporterHelper::SchemaExporterHelper(const QString &schemaName,
                                           IQueryScheduler *queryScheduler,
                                           DbTreeModel *model,
                                           SchemaExportOptions *options,
                                           QObject *parent) :
    SchemaOperationHelper(schemaName, queryScheduler, model, parent),
    options(options)
{
}

SchemaExporterHelper::~SchemaExporterHelper()
{
    delete options;
}

void SchemaExporterHelper::stop()
{
    SchemaOperationHelper::stop();
}

void SchemaExporterHelper::startComparer(DbTreeModel::DbTreeNodeType parentNodeType, const QStringList &checkedChildNames)
{
    workerThread = new SchemaExporterThread(parentNodeType,
                                            sourceSchema,
                                            checkedChildNames,
                                            options,
                                            sourceScheduler,
                                            this);

    connect(workerThread, SIGNAL(statusChanged(QString)), this, SIGNAL(statusChanged(QString)));
    connect(workerThread, SIGNAL(exportCompleted()), this, SLOT(compareNextParentItem()));
    connect(workerThread, SIGNAL(objectExported(DbTreeModel::DbTreeNodeType,QString)), this, SLOT(objectExported(DbTreeModel::DbTreeNodeType,QString)));
    connect(workerThread, SIGNAL(exportError(QString,OciException)), this, SLOT(subComparisonError(QString,OciException)));

    workerThread->start();
}

void SchemaExporterHelper::objectExported(DbTreeModel::DbTreeNodeType parentNodeType, const QString &objectName)
{
    emit chunkCompleted(1);
}

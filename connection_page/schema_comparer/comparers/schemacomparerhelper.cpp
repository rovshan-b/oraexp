#include "schemacomparerhelper.h"
#include "dbobjectscomparehelper.h"
#include "defines.h"
#include <QDebug>

SchemaComparerHelper::SchemaComparerHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                           const QString &targetSchema, IQueryScheduler *targetScheduler,
                                           DbTreeModel *model, SchemaComparisonOptions *options, QObject *parent) :
    SchemaOperationHelper(sourceSchema, sourceScheduler, model, parent),
    targetSchema(targetSchema),
    targetScheduler(targetScheduler),
    dbObjectsComparer(options, this)
{
    connect(&dbObjectsComparer, SIGNAL(statusChanged(QString)), this, SIGNAL(statusChanged(QString)));
    connect(&dbObjectsComparer, SIGNAL(ddlReady(QString)), this, SIGNAL(ddlReady(QString)));
    connect(&dbObjectsComparer, SIGNAL(comparisonCompleted()), this, SLOT(compareNextParentItem()));
    connect(&dbObjectsComparer, SIGNAL(chunkCompleted(int)), this, SIGNAL(chunkCompleted(int)));
    connect(&dbObjectsComparer, SIGNAL(comparisonError(QString,OciException)), this, SLOT(subComparisonError(QString,OciException)));
    connect(&dbObjectsComparer, SIGNAL(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)), this, SIGNAL(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)));
}

SchemaComparerHelper::~SchemaComparerHelper()
{
    qDebug("deleted SchemaComparerHelper");
}

void SchemaComparerHelper::compare()
{
    dbObjectsComparer.resetState();

    SchemaOperationHelper::compare();
}

void SchemaComparerHelper::stop()
{
    SchemaOperationHelper::stop();
    dbObjectsComparer.stop();
}

void SchemaComparerHelper::startComparer(DbTreeModel::DbTreeNodeType parentNodeType, const QStringList &checkedChildNames)
{
    dbObjectsComparer.compare(this->sourceSchema, this->sourceScheduler,
                               this->targetSchema, this->targetScheduler,
                               checkedChildNames, parentNodeType);
}

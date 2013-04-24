#include "schemaoperationhelper.h"
#include "navtree/dbtreemodelnodeexpander.h"
#include "navtree/dbtreeitem.h"

SchemaOperationHelper::SchemaOperationHelper(const QString &schemaName, IQueryScheduler *queryScheduler, DbTreeModel *model, QObject *parent) :
    QObject(parent),
    sourceSchema(schemaName),
    sourceScheduler(queryScheduler),
    model(model)
{
    createParentIndexReplacements();
}

void SchemaOperationHelper::compare()
{
    emitStatusChanged(tr("Loading object list..."));

    DbTreeModelNodeExpander *expander = new DbTreeModelNodeExpander(model);
    expander->setLoadOnlyChecked();
    connect(expander, SIGNAL(loadComplete()), this, SLOT(objectListLoaded()));
    connect(expander, SIGNAL(loadError(OciException)), this, SLOT(objectListLoadError(OciException)));
    expander->loadChildren(QModelIndex());
}

void SchemaOperationHelper::objectListLoaded()
{
    populateParentIndexesToCompare();

    if(parentIndexesToCompare.isEmpty()){
        emitCompletedSignal();
        return;
    }

    compareNextParentItem();
}

void SchemaOperationHelper::objectListLoadError(const OciException &exception)
{
    subComparisonError("populate_tree_child_nodes", exception);
}


void SchemaOperationHelper::populateParentIndexesToCompare()
{
    Q_ASSERT(parentIndexesToCompare.isEmpty());

    int objectCount = 0;

    QModelIndex invalidIndex=QModelIndex();
    int rowCount=model->rowCount(invalidIndex);
    QModelIndex childIndex;
    DbTreeItem *item;
    for(int i=0; i<rowCount; ++i){
        childIndex=model->index(i, 0, invalidIndex);
        item=static_cast<DbTreeItem *>(childIndex.internalPointer());
        if(item->checkState()!=Qt::Unchecked){
            int replacementCount=getReplacementCount(item->getItemType());
            for(int k=0; k<replacementCount; ++k){
                parentIndexesToCompare.enqueue(childIndex);

                objectCount += model->getCheckedChildCount(childIndex);
            }
        }
    }

    emit objectCountDetermined(objectCount);
}

void SchemaOperationHelper::compareNextParentItem()
{
    if(parentIndexesToCompare.isEmpty() || this->stopped){
        emitCompletedSignal();
        return;
    }

    currentParentIndex=parentIndexesToCompare.dequeue();
    compareChildItems();
}

void SchemaOperationHelper::compareChildItems()
{
    DbTreeItem *currentParentItem=static_cast<DbTreeItem *>(currentParentIndex.internalPointer());
    Q_ASSERT(currentParentItem);

    DbTreeModel::DbTreeNodeType parentNodeType=currentParentItem->getItemType();
    parentNodeType=getNextReplacement(parentNodeType);

    QStringList checkedChildNames=model->getCheckedChildNames(currentParentIndex);
    if(checkedChildNames.isEmpty()){
        compareNextParentItem();
        return;
    }

    startComparer(parentNodeType, checkedChildNames);
}

void SchemaOperationHelper::createParentIndexReplacements()
{
    QQueue<DbTreeModel::DbTreeNodeType> packageReplacements;
    packageReplacements.enqueue(DbTreeModel::PackageSpecs);
    packageReplacements.enqueue(DbTreeModel::PackageBodies);
    parentIndexReplacements[DbTreeModel::Packages]=packageReplacements;

    QQueue<DbTreeModel::DbTreeNodeType> typeReplacements;
    typeReplacements.enqueue(DbTreeModel::TypeSpecs);
    typeReplacements.enqueue(DbTreeModel::TypeBodies);
    parentIndexReplacements[DbTreeModel::Types]=typeReplacements;
}

int SchemaOperationHelper::getReplacementCount(DbTreeModel::DbTreeNodeType parentNodeType) const
{
    if(parentIndexReplacements.contains(parentNodeType)){
        return parentIndexReplacements.value(parentNodeType).size();
    }else{
        return 1;
    }
}

DbTreeModel::DbTreeNodeType SchemaOperationHelper::getNextReplacement(DbTreeModel::DbTreeNodeType parentNodeType)
{
    if(parentIndexReplacements.contains(parentNodeType)){
        return parentIndexReplacements[parentNodeType].dequeue();
    }else{
        return parentNodeType;
    }
}

void SchemaOperationHelper::emitStatusChanged(const QString &newStatus)
{
    emit statusChanged(newStatus);
}

void SchemaOperationHelper::subComparisonError(const QString &taskName, const OciException &exception)
{
    emit comparisonError(taskName, exception);

    this->deleteLater();
}

void SchemaOperationHelper::emitCompletedSignal()
{
    emit completed();

    this->deleteLater();
}

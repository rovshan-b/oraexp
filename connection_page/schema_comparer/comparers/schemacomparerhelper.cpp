#include "schemacomparerhelper.h"
#include "dbobjectscomparehelper.h"
#include "defines.h"
#include <QDebug>

SchemaComparerHelper::SchemaComparerHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                           const QString &targetSchema, IQueryScheduler *targetScheduler,
                                           DbTreeModel *model, SchemaComparisonOptions *options, QObject *parent) :
    QObject(parent),
    sourceSchema(sourceSchema), targetSchema(targetSchema),
    sourceScheduler(sourceScheduler), targetScheduler(targetScheduler),
    model(model),
    dbObjectsComparer(options, this)
{
    connect(model, SIGNAL(childrenPopulated(QModelIndex)), this, SLOT(childrenPopulated(QModelIndex)));
    connect(model, SIGNAL(childrenPopulateError(QModelIndex,OciException)), this, SLOT(childrenPopulateError(QModelIndex,OciException)));

    connect(&dbObjectsComparer, SIGNAL(statusChanged(QString)), this, SIGNAL(statusChanged(QString)));
    connect(&dbObjectsComparer, SIGNAL(ddlReady(QString)), this, SIGNAL(ddlReady(QString)));
    connect(&dbObjectsComparer, SIGNAL(comparisonCompleted()), this, SLOT(objectComparisonCompleted()));
    connect(&dbObjectsComparer, SIGNAL(chunkCompleted(int)), this, SIGNAL(chunkCompleted(int)));
    connect(&dbObjectsComparer, SIGNAL(comparisonError(QString,OciException)), this, SLOT(subComparisonError(QString,OciException)));
    connect(&dbObjectsComparer, SIGNAL(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)), this, SIGNAL(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)));

    createParentIndexReplacements();
}

SchemaComparerHelper::~SchemaComparerHelper()
{
    qDebug("deleted SchemaComparerHelper");
}

void SchemaComparerHelper::compare()
{
    emit statusChanged(tr("Starting..."));

    dbObjectsComparer.resetState();

    totalItemCountToCompare=0;
    populateParentIndexesToCompare();

    if(parentIndexesToCompare.isEmpty()){
        emitCompletedSignal();
        return;
    }

    loadNextItemList();
}

void SchemaComparerHelper::stop()
{
    IDbObjectCompareHelper::stop();
    dbObjectsComparer.stop();
}

void SchemaComparerHelper::populateParentIndexesToCompare()
{
    Q_ASSERT(parentIndexesToCompare.isEmpty());

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
            }
        }
    }
}

void SchemaComparerHelper::loadNextItemList()
{
    if(parentIndexesToCompare.isEmpty()){
        populateParentIndexesToCompare();
        emit objectCountDetermined(totalItemCountToCompare);
        compareNextItem();
        return;
    }

    currentParentIndex=parentIndexesToCompare.dequeue();
    DbTreeItem *currentParentItem=static_cast<DbTreeItem *>(currentParentIndex.internalPointer());
    bool needToLoadList=!currentParentItem->areChildrenPopulated();
    if(needToLoadList){
        loadItemList();
    }else{
        totalItemCountToCompare += getCheckedChildCount(currentParentIndex);
        loadNextItemList();
    }
}

void SchemaComparerHelper::compareNextItem()
{
    if(parentIndexesToCompare.isEmpty() || this->stopped){
        emitCompletedSignal();
        return;
    }

    currentParentIndex=parentIndexesToCompare.dequeue();
    compareItems();
}

void SchemaComparerHelper::loadItemList()
{
    statusChanged("Loading object list...");
    model->loadChildItems(currentParentIndex);
}

void SchemaComparerHelper::compareItems()
{
    DbTreeItem *currentParentItem=static_cast<DbTreeItem *>(currentParentIndex.internalPointer());
    Q_ASSERT(currentParentItem);

    DbTreeModel::DbTreeNodeType parentNodeType=currentParentItem->getItemType();
    parentNodeType=getNextReplacement(parentNodeType);

    QStringList checkedChildNames=getCheckedChildNames(currentParentIndex);
    if(checkedChildNames.isEmpty()){
        compareNextItem();
        return;
    }

    dbObjectsComparer.compare(this->sourceSchema, this->sourceScheduler,
                               this->targetSchema, this->targetScheduler,
                               checkedChildNames, parentNodeType);
}

QStringList SchemaComparerHelper::getCheckedChildNames(const QModelIndex &parent)
{
    QStringList result;

    QList<DbTreeItem*> items=model->getChildItems(parent);
    foreach(DbTreeItem *item, items){
        if(item->checkState()==Qt::Checked && item->canGenerateDdlForItem()){
            result.append(item->itemName());
        }
    }

    return result;
}

int SchemaComparerHelper::getCheckedChildCount(const QModelIndex &parent)
{
    int result=0;

    QList<DbTreeItem*> items=model->getChildItems(parent);
    foreach(DbTreeItem *item, items){
        if(item->checkState()==Qt::Checked && item->canGenerateDdlForItem()){
            ++result;
        }
    }

    return result;
}

void SchemaComparerHelper::createParentIndexReplacements()
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

int SchemaComparerHelper::getReplacementCount(DbTreeModel::DbTreeNodeType parentNodeType) const
{
    if(parentIndexReplacements.contains(parentNodeType)){
        return parentIndexReplacements.value(parentNodeType).size();
    }else{
        return 1;
    }
}

DbTreeModel::DbTreeNodeType SchemaComparerHelper::getNextReplacement(DbTreeModel::DbTreeNodeType parentNodeType)
{
    if(parentIndexReplacements.contains(parentNodeType)){
        Q_ASSERT(parentIndexReplacements[parentNodeType].size()>0);
        return parentIndexReplacements[parentNodeType].dequeue();
    }else{
        return parentNodeType;
    }
}

void SchemaComparerHelper::childrenPopulated(const QModelIndex &parent)
{
    if(parent==currentParentIndex){
        totalItemCountToCompare += getCheckedChildCount(currentParentIndex);
        loadNextItemList();
    }
}

void SchemaComparerHelper::childrenPopulateError(const QModelIndex &, const OciException &exception)
{
    subComparisonError("populate_tree_child_nodes", exception);
}

void SchemaComparerHelper::objectComparisonCompleted()
{
    compareNextItem();
}

void SchemaComparerHelper::subComparisonError(const QString &taskName, const OciException &exception)
{
    emit comparisonError(taskName, exception);

    this->deleteLater();
}

void SchemaComparerHelper::emitCompletedSignal()
{
    emit completed();

    this->deleteLater();
}

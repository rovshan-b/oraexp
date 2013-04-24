#include "dbtreemodelnodeexpander.h"
#include "dbtreeitem.h"

DbTreeModelNodeExpander::DbTreeModelNodeExpander(DbTreeModel *treeModel) :
    QObject(treeModel), currentIx(0), loadOnlyChecked(false)
{
    connect(treeModel, SIGNAL(childrenPopulated(QModelIndex)), this, SLOT(childrenPopulated(QModelIndex)));
    connect(treeModel, SIGNAL(childrenPopulateError(QModelIndex,OciException)), this, SLOT(childrenPopulateError(QModelIndex,OciException)));
}

void DbTreeModelNodeExpander::loadChildren(const QModelIndex &parent,
                                           QList<DbTreeModel::DbTreeNodeType> nodeTypes)
{
    parentIndex = parent;
    nodeTypesToLoad = nodeTypes;
    currentIx = -1;


    load();
}

void DbTreeModelNodeExpander::setLoadOnlyChecked()
{
    this->loadOnlyChecked=true;
}

void DbTreeModelNodeExpander::load()
{
    ++currentIx;

    DbTreeModel *treeModel = static_cast<DbTreeModel*>(this->parent());
    int rowCount = treeModel->rowCount(parentIndex);


    DbTreeItem *item;
    QModelIndex index;
    int i;
    bool loading=false;
    for(i=currentIx; i<rowCount; ++i){
        index = treeModel->index(i,0,parentIndex);
        item = static_cast<DbTreeItem*>(index.internalPointer());
        if(!item->areChildrenPopulated() && item->hasChildren() &&
                (nodeTypesToLoad.isEmpty() || nodeTypesToLoad.contains(item->getItemType())) &&
                (loadOnlyChecked==false || item->checkState()!=Qt::Unchecked)){
            treeModel->loadChildItems(index);
            loading=true;
            break;
        }
    }

    currentIx = i;

    if(!loading){
        emit loadComplete();

        this->deleteLater();
    }
}

void DbTreeModelNodeExpander::childrenPopulated(const QModelIndex & /*parent*/)
{
    load();
}

void DbTreeModelNodeExpander::childrenPopulateError(const QModelIndex & /*parent*/, const OciException &exception)
{
    emit loadError(exception);

    this->deleteLater();
}

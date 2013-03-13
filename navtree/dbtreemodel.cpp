#include "dbtreemodel.h"
#include "dbtreeitem.h"
#include "loadingdbtreeitem.h"
#include "schema/dbschemaitem.h"
#include <iostream>
#include "interfaces/iqueryscheduler.h"
#include "nodepopulatorthread.h"
#include "idbtreeitemcheckstateprovider.h"
#include <QDebug>

using namespace std;

DbTreeModel::DbTreeModel(IQueryScheduler *queryScheduler, const QString &defaultSchemaName, QObject *parent) :
    QAbstractItemModel(parent),
    m_queryScheduler(queryScheduler),
    m_nodePopulatorThread(0),
    m_fetchInProgress(false),
    m_displayCheckBoxes(false),
    m_onlyTopLevelCheckable(true),
    m_checkStateProvider(0)
{
    m_rootItem = new DbSchemaItem(tr("Database object name"), "", this, 0, true);
    m_rootItem->setSchemaName(defaultSchemaName);
}

DbTreeModel::~DbTreeModel()
{
    deleteNodePopulatorThread();
    delete m_rootItem;
}

DbConnection *DbTreeModel::getDb() const
{
    return m_queryScheduler->getDb();
}

void DbTreeModel::setUiManager(DbUiManager *uiManager)
{
    this->m_uiManager=uiManager;
}

DbUiManager *DbTreeModel::getUiManager() const
{
    return this->m_uiManager;
}

int DbTreeModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid()){
        return static_cast<DbTreeItem*>(parent.internalPointer())->columnCount();
    }else{
        return m_rootItem->columnCount();
    }
}

bool DbTreeModel::isValidIndex(const QModelIndex &index) const
{
    if(!index.isValid()){
        return false;
    }

    int totalRowCount=index.parent().isValid() ? rowCount(index.parent()) : m_rootItem->childCount();
    int totalColCount=1;

    if(index.row()>=totalRowCount ||
            index.column()>=totalColCount){
        return false;
    }

    return true;
}

QVariant DbTreeModel::data(const QModelIndex &index, int role) const
{
    if(!isValidIndex(index)){
        return QVariant();
    }

    DbTreeItem *item = static_cast<DbTreeItem *>(index.internalPointer());

    if(role==Qt::CheckStateRole &&
            checkboxesEnabled() &&
            (!isCheckable(item) || (m_onlyTopLevelCheckable && !item->isTopLevel()))){
        return QVariant();
    }

    return item->data(index.column(), role);
}

bool DbTreeModel::setData (const QModelIndex &index, const QVariant &value, int role)
{
    if(!isValidIndex(index)){
        return false;
    }

    if(role==Qt::CheckStateRole && m_displayCheckBoxes){
        int newCheckState=value.toInt();
        return checkItem(index, newCheckState==0 ? Qt::Unchecked : Qt::Checked, true);
    }

    return false;
}

bool DbTreeModel::checkItem(const QModelIndex &index, Qt::CheckState newCheckState, bool updateParentItem)
{
    DbTreeItem *item = static_cast<DbTreeItem *>(index.internalPointer());
    Q_ASSERT(item);

    if(!isCheckable(item)){
        return false;
    }

    DbTreeItem::CheckBehavior checkBehavior=item->checkBehavior();
    if(checkBehavior==DbTreeItem::Checkable){
        item->setCheckState(newCheckState);
        if(updateParentItem){
            emit dataChanged(index, index);
            updateParentCheckedState(index);
        }
        return true;
    }else if(checkBehavior==DbTreeItem::CheckableForTogglingChildren){
        item->setCheckState(newCheckState);
        emit dataChanged(index, index);

        DbTreeItem *item;
        QModelIndex childIndex;
        int childCount=rowCount(index);
        for(int i=0; i<childCount; ++i){
            childIndex=index.child(i,0);
            item=static_cast<DbTreeItem *>(childIndex.internalPointer());
            if(item!=0 && item->checkBehavior()==DbTreeItem::Checkable && item->checkState()!=newCheckState){
                checkItem(childIndex, newCheckState, false);
            }
        }

        if(childCount>0){
            emit dataChanged(index.child(0,0),index.child(childCount-1,0));
        }
    }

    return false;
}

void DbTreeModel::updateParentCheckedState(const QModelIndex &index)
{
    QModelIndex parentIndex=index.parent();
    if(!isValidIndex(parentIndex)){
        return;
    }

    if(rowCount(parentIndex)==0){
        return;
    }

    DbTreeItem *parentItem = static_cast<DbTreeItem *>(parentIndex.internalPointer());

    DbTreeItem::CheckBehavior parentCheckBehavior=parentItem->checkBehavior();
    if(parentCheckBehavior!=DbTreeItem::CheckableForTogglingChildren){
        return;
    }

    bool hasChecked=false;
    bool hasUnchecked=false;
    DbTreeItem *item;
    QModelIndex childIndex;
    for(int i=0; i<rowCount(parentIndex); ++i){

        if(hasChecked && hasUnchecked){
            break;
        }

        childIndex=parentIndex.child(i,0);
        item=static_cast<DbTreeItem *>(childIndex.internalPointer());
        if(item!=0 && item->checkBehavior()==DbTreeItem::Checkable){
            Qt::CheckState checkState=item->checkState();
            if(checkState==Qt::Checked && !hasChecked){
                hasChecked=true;
            }else if(checkState==Qt::Unchecked && !hasUnchecked){
                hasUnchecked=true;
            }
        }
    }

    Qt::CheckState newCheckState;
    if(hasChecked && hasUnchecked){
        newCheckState=Qt::PartiallyChecked;
    }else if(hasChecked){
        newCheckState=Qt::Checked;
    }else{
        newCheckState=Qt::Unchecked;
    }

    if(newCheckState!=parentItem->checkState()){
        parentItem->setCheckState(newCheckState);
        emit dataChanged(parentIndex, parentIndex);
    }
}

Qt::ItemFlags DbTreeModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return 0;
    }

    Qt::ItemFlags flags=Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if(m_displayCheckBoxes){
        DbTreeItem *item = static_cast<DbTreeItem*>(index.internalPointer());
        Q_ASSERT(item);

        if(item->checkBehavior()!=DbTreeItem::NotCheckable && isCheckable(item) &&
                (m_onlyTopLevelCheckable && item->isTopLevel())){
            flags |= Qt::ItemIsUserCheckable;
        }
    }

    return flags;
}

QVariant DbTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole){
        return m_rootItem->data(section, Qt::DisplayRole);
    }

    return QVariant();
}

QModelIndex DbTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent)){
        return QModelIndex();
    }

    DbTreeItem *parentItem;

    if(!parent.isValid()){
        parentItem=m_rootItem;
    }else{
        parentItem=static_cast<DbTreeItem *>(parent.internalPointer());
    }

    DbTreeItem *childItem=parentItem->child(row);

    if(childItem){
        return createIndex(row, column, childItem);
    }else{
        return QModelIndex();
    }
}

QModelIndex DbTreeModel::parent(const QModelIndex &child) const
{
    if(!child.isValid()){
        return QModelIndex();
    }

    DbTreeItem *childItem = static_cast<DbTreeItem *>(child.internalPointer());

    if(childItem==m_rootItem){
        return QModelIndex();
    }

    DbTreeItem *parentItem = childItem->parent();

    if(parentItem==m_rootItem || parentItem==0){
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int DbTreeModel::rowCount(const QModelIndex &parent) const
{
    DbTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DbTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

bool DbTreeModel::hasChildren(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return true;
    }else{
        DbTreeItem *node=static_cast<DbTreeItem*>(parent.internalPointer());
        bool result = node->hasChildren();
        return result;
    }

}

bool DbTreeModel::canFetchMore(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return false;
    }else{
        DbTreeItem *node=static_cast<DbTreeItem*>(parent.internalPointer());
        bool result = !node->areChildrenPopulated() && /*!db->isBusy() &&*/ !this->m_fetchInProgress;
        return result;
    }
}

void DbTreeModel::fetchMore (const QModelIndex & parent)
{
    if(!parent.isValid()){
        return;
    }

    populateChildNodes(parent);
}

void DbTreeModel::populateChildNodes(const QModelIndex & parent)
{
    DbTreeItem *node=static_cast<DbTreeItem*>(parent.internalPointer());
    if(node->areChildrenPopulated()){
        return;
    }

    if(this->m_fetchInProgress){
        return;
    }

    this->m_fetchInProgress=true;

    loadChildItems(parent);
}

void DbTreeModel::loadChildItems(const QModelIndex &parent)
{
    DbTreeItem *node=static_cast<DbTreeItem*>(parent.internalPointer());
    if(node->areChildrenPopulated()){
        return;
    }

    node->setChildrenPopulated(true);

    startNodePopulatorThread(parent);
}

void DbTreeModel::nodeListAvailable(const DbTreeItemResult &result)
{
    DbTreeItem *node=static_cast<DbTreeItem*>(result.parentNode.internalPointer());
    clearChildren(result.parentNode, false);

    if(result.hasError){
        emit childrenPopulateError(result.parentNode, result.exception);
        deleteNodePopulatorThread();
        this->m_fetchInProgress=false;
        return;
    }

    QList<DbTreeItem*> childNodes=result.childNodes;
    int newItemCount=childNodes.size();
    int currentChildCount=node->childCount();
    DbTreeItem *child;
    if(newItemCount>0){
        beginInsertRows(result.parentNode, currentChildCount, currentChildCount+newItemCount-1);

        for(int i=0; i<newItemCount; ++i){
            child=childNodes.at(i);
            node->appendChild(child);
            if(child->hasChildren()){
                child->appendChild(new LoadingDbTreeItem(this, node));
            }
        }

        endInsertRows();
    }

    if(node->displayChildCount()){
        QString itemText=node->itemText();
        int lastBracketIx=itemText.lastIndexOf("(");
        if(lastBracketIx!=-1){
            itemText.truncate(lastBracketIx-1);
        }
        node->setItemText(itemText.append(" (%1)").arg(newItemCount));
    }

    deleteNodePopulatorThread();

    this->m_fetchInProgress=false;

    if(node==m_rootItem){
        reset();
    }

    emit childrenPopulated(result.parentNode);
}

void DbTreeModel::startNodePopulatorThread(const QModelIndex &parent)
{
    Q_ASSERT(m_nodePopulatorThread==0);

    m_queryScheduler->increaseRefCount();

    m_nodePopulatorThread=new NodePopulatorThread(parent, this);
    connect(m_nodePopulatorThread, SIGNAL(queryCompleted(const DbTreeItemResult &)), this, SLOT(nodeListAvailable(const DbTreeItemResult &)));
    m_nodePopulatorThread->start();
}

void DbTreeModel::deleteNodePopulatorThread()
{
    if(m_nodePopulatorThread!=0){
        m_nodePopulatorThread->wait();
        delete m_nodePopulatorThread;
        m_queryScheduler->decreaseRefCount();
        m_nodePopulatorThread=0;
    }
}

void DbTreeModel::clearChildren(const QModelIndex & parent, bool resetPopulatedFlag)
{
    if(!parent.isValid()){
        return;
    }

    DbTreeItem *node=static_cast<DbTreeItem*>(parent.internalPointer());
    int childCount=node->childCount();
    if(childCount>0){
        beginRemoveRows(parent, 0, childCount-1);
        node->clearChildItems(resetPopulatedFlag);
        endRemoveRows();
    }
}

void DbTreeModel::refreshChildren(const QModelIndex & parent)
{
    clearChildren(parent, true);
    populateChildNodes(parent);
}

void DbTreeModel::setupInitialItems(DbTreeNodeTypes nodeTypesToDisplay)
{
    m_rootItem->setNodeTypesToDisplay(nodeTypesToDisplay);
    populateChildNodes(createIndex(0,0,m_rootItem));
}

void DbTreeModel::checkAll(const QModelIndex &parent, bool check)
{
    DbTreeItem *item;
    QModelIndex childIndex;
    int childCount=rowCount(parent);
    Qt::CheckState newCheckState=check ? Qt::Checked : Qt::Unchecked;

    //DbTreeItem *parentItem=parent.isValid() ? static_cast<DbTreeItem *>(parent.internalPointer()) : rootItem;

    for(int i=0; i<childCount; ++i){
        childIndex=index(i,0,parent);
        item=static_cast<DbTreeItem *>(childIndex.internalPointer());
        if(item!=0 && item->checkState()!=newCheckState){
            checkItem(childIndex, newCheckState, false);
        }
    }
}

bool DbTreeModel::checkItem(const QModelIndex &index, Qt::CheckState newCheckState)
{
    return this->checkItem(index, newCheckState, true);
}

QList<QModelIndex> DbTreeModel::getCheckedItems(const QModelIndex &parent, DbTreeModel::DbTreeNodeType nodeType) const
{
    QList<QModelIndex> results;

    QModelIndex itemIndex=getChildIndex(parent, nodeType);
    int rows=rowCount(itemIndex);

    QModelIndex childIndex;
    DbTreeItem *item;
    for(int i=0; i<rows; ++i){
        childIndex = index(i, 0, itemIndex);
        item=static_cast<DbTreeItem *>(childIndex.internalPointer());
        if(item->checkState()!=Qt::Unchecked){
            results.append(childIndex);
        }
    }

    return results;
}

bool DbTreeModel::isCheckable(DbTreeItem *item) const
{
    Q_ASSERT(item);
    Q_ASSERT(item->parent());

    if(m_checkStateProvider!=0 && !m_checkStateProvider->isCheckable(item)){
        return false;
    }

    return m_checkableItemTypes.testFlag(item->getItemType()) &&
            (item->parent()==m_rootItem || m_checkableItemTypes.testFlag(item->parent()->getItemType()));
}

QList<DbTreeItem*> DbTreeModel::getChildItems(const QModelIndex &parent, bool checkType, DbTreeNodeType nodeType) const
{
    QList<DbTreeItem*> results;

    DbTreeItem *parentItem=m_rootItem;
    if(parent.isValid()){
        parentItem=static_cast<DbTreeItem *>(parent.internalPointer());
    }

    DbTreeItem *childItem;
    for(int i=0; i<parentItem->childCount(); ++i){
        childItem=parentItem->child(i);
        if(!checkType || childItem->getItemType()==nodeType){
            results.append(childItem);
        }
    }

    return results;
}

QList<QModelIndex> DbTreeModel::getChildIndexes(const QModelIndex &parent, DbTreeNodeType nodeType) const
{
    QList<QModelIndex> results;

    DbTreeItem *parentItem=m_rootItem;
    if(parent.isValid()){
        parentItem=static_cast<DbTreeItem *>(parent.internalPointer());
    }

    DbTreeItem *childItem;
    int parentRowCount=rowCount(parent);
    for(int i=0; i<parentRowCount; ++i){
        childItem=parentItem->child(i);
        if(childItem->getItemType()==nodeType){
            results.append(index(i,0,parent));
        }
    }

    return results;
}

QModelIndex DbTreeModel::getChildIndex(const QModelIndex &parent, DbTreeNodeType nodeType) const
{
    DbTreeItem *parentItem=m_rootItem;
    if(parent.isValid()){
        parentItem=static_cast<DbTreeItem *>(parent.internalPointer());
    }

    DbTreeItem *childItem;
    int parentRowCount=rowCount(parent);
    for(int i=0; i<parentRowCount; ++i){
        childItem=parentItem->child(i);
        if(childItem->getItemType()==nodeType){
            return index(i,0,parent);
        }
    }

    Q_ASSERT(false);

    return QModelIndex();
}

void DbTreeModel::setCheckStateProvider(IDbTreeItemCheckStateProvider *provider)
{
    m_checkStateProvider=provider;
}

bool DbTreeModel::isCheckable(const QModelIndex &index) const
{
    DbTreeItem *item=static_cast<DbTreeItem *>(index.internalPointer());
    Q_ASSERT(item);

    return this->isCheckable(item);
}

QString DbTreeModel::itemName(const QModelIndex &index) const
{
    DbTreeItem *item=static_cast<DbTreeItem *>(index.internalPointer());
    Q_ASSERT(item);

    return item->itemName();
}

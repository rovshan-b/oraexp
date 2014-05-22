#include "codestructuremodel.h"
#include "codestructuretreeitem.h"
#include "code_parser/plsql/plsqltreebuilder.h"

CodeStructureModel::CodeStructureModel(PlSqlTreeBuilder *treeBuilder, QObject *parent) :
    QAbstractItemModel(parent),
    treeBuilder(treeBuilder)
{
    rootItem = new CodeStructureTreeItem(treeBuilder->findFirstMultiChildNode());
}

CodeStructureModel::~CodeStructureModel()
{
    delete rootItem;
}

QVariant CodeStructureModel::data(const QModelIndex &index, int role) const
{
    if(!isValidIndex(index)){
        return QVariant();
    }

    CodeStructureTreeItem *item = static_cast<CodeStructureTreeItem *>(index.internalPointer());

    return item->data(role);
}

QModelIndex CodeStructureModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent)){
        return QModelIndex();
    }

    CodeStructureTreeItem *parentItem;

    if(!parent.isValid()){
        parentItem=rootItem;
    }else{
        parentItem=static_cast<CodeStructureTreeItem *>(parent.internalPointer());
    }

    CodeStructureTreeItem *childItem=parentItem->child(row);

    if(childItem){
        return createIndex(row, column, childItem);
    }else{
        return QModelIndex();
    }
}

QModelIndex CodeStructureModel::parent(const QModelIndex &child) const
{
    if(!child.isValid()){
        return QModelIndex();
    }

    CodeStructureTreeItem *childItem = static_cast<CodeStructureTreeItem *>(child.internalPointer());

    if(childItem==rootItem){
        return QModelIndex();
    }

    CodeStructureTreeItem *parentItem = childItem->parent();

    if(parentItem==rootItem || parentItem==0){
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int CodeStructureModel::rowCount(const QModelIndex &parent) const
{
    CodeStructureTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<CodeStructureTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int CodeStructureModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1;
}

bool CodeStructureModel::hasChildren(const QModelIndex &parent) const
{
    if(!parent.isValid()){
        return true;
    }else{
        CodeStructureTreeItem *node=static_cast<CodeStructureTreeItem*>(parent.internalPointer());
        bool result = node->hasChildren();
        return result;
    }
}

bool CodeStructureModel::isValidIndex(const QModelIndex &index) const
{
    if(!index.isValid()){
        return false;
    }

    int totalRowCount=index.parent().isValid() ? rowCount(index.parent()) : rootItem->childCount();
    int totalColCount=1;

    if(index.row()>=totalRowCount ||
            index.column()>=totalColCount){
        return false;
    }

    return true;
}

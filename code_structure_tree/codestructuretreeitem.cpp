#include "codestructuretreeitem.h"
#include "beans/parsetreenode.h"

CodeStructureTreeItem::CodeStructureTreeItem(ParseTreeNode *node) :
    node(node)
{
}

CodeStructureTreeItem::~CodeStructureTreeItem()
{
    qDeleteAll(childItems);
}

QVariant CodeStructureTreeItem::data(int role) const
{
    return QVariant();
}

int CodeStructureTreeItem::row() const
{
    if(parentItem){
        return parentItem->childItems.indexOf(const_cast<CodeStructureTreeItem*>(this));
    }

    return 0;
}

bool CodeStructureTreeItem::hasChildren() const
{
    return true;
}

CodeStructureTreeItem *CodeStructureTreeItem::child(int row)
{
    return childItems.at(row);
}

int CodeStructureTreeItem::childCount() const
{
    return childItems.count();
}

CodeStructureTreeItem *CodeStructureTreeItem::parent() const
{
    return parentItem;
}

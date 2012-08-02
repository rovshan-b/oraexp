#include "dbpackagebodyitem.h"

DbPackageBodyItem::DbPackageBodyItem(const QString &itemText, const QString &itemName,
                                            DbTreeModel *model,
                                            DbTreeItem *parent) :
                                            DbTreeItem(itemText, itemName, model, parent)
{

}


QString DbPackageBodyItem::iconName() const
{
    return "package";
}

bool DbPackageBodyItem::hasChildren() const
{
    return false;
}


bool DbPackageBodyItem::displayChildCount() const
{
    return false;
}

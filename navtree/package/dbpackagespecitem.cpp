#include "dbpackagespecitem.h"

DbPackageSpecItem::DbPackageSpecItem(const QString &itemText, const QString &itemName,
                                            DbTreeModel *model,
                                            DbTreeItem *parent) :
                                            DbTreeItem(itemText, itemName, model, parent)
{

}


QString DbPackageSpecItem::iconName() const
{
    return "package";
}

bool DbPackageSpecItem::hasChildren() const
{
    return false;
}


bool DbPackageSpecItem::displayChildCount() const
{
    return false;
}

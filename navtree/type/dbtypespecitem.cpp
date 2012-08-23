#include "dbtypespecitem.h"

DbTypeSpecItem::DbTypeSpecItem(const QString &itemText, const QString &itemName,
                                            DbTreeModel *model,
                                            DbTreeItem *parent) :
                                            DbTreeItem(itemText, itemName, model, parent)
{

}


QString DbTypeSpecItem::iconName() const
{
    return "type";
}

bool DbTypeSpecItem::hasChildren() const
{
    return false;
}


bool DbTypeSpecItem::displayChildCount() const
{
    return false;
}

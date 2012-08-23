#include "dbtypebodyitem.h"

DbTypeBodyItem::DbTypeBodyItem(const QString &itemText, const QString &itemName,
                                            DbTreeModel *model,
                                            DbTreeItem *parent) :
                                            DbTreeItem(itemText, itemName, model, parent)
{

}


QString DbTypeBodyItem::iconName() const
{
    return "type";
}

bool DbTypeBodyItem::hasChildren() const
{
    return false;
}


bool DbTypeBodyItem::displayChildCount() const
{
    return false;
}

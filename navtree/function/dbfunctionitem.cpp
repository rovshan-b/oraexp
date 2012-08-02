#include "dbfunctionitem.h"
#include "util/strutil.h"

DbFunctionItem::DbFunctionItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbFunctionItem::iconName() const
{
    return "function";
}

bool DbFunctionItem::hasChildren() const
{
    return false;
}

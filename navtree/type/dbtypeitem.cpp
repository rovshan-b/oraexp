#include "dbtypeitem.h"
#include "util/strutil.h"

DbTypeItem::DbTypeItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbTypeItem::iconName() const
{
    return "procedure";
}

bool DbTypeItem::hasChildren() const
{
    return false;
}

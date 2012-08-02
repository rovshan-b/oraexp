#include "dbdblinkitem.h"
#include "util/strutil.h"

DbDbLinkItem::DbDbLinkItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbDbLinkItem::iconName() const
{
    return "dblink";
}

bool DbDbLinkItem::hasChildren() const
{
    return false;
}

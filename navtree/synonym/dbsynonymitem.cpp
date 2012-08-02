#include "dbsynonymitem.h"
#include "util/strutil.h"

DbSynonymItem::DbSynonymItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbSynonymItem::iconName() const
{
    return "synonym";
}

bool DbSynonymItem::hasChildren() const
{
    return false;
}

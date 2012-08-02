#include "dbsequenceitem.h"
#include "util/strutil.h"

DbSequenceItem::DbSequenceItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbSequenceItem::iconName() const
{
    return "sequence";
}

bool DbSequenceItem::hasChildren() const
{
    return false;
}

#include "dbprocedureitem.h"
#include "util/strutil.h"

DbProcedureItem::DbProcedureItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbProcedureItem::iconName() const
{
    return "procedure";
}

bool DbProcedureItem::hasChildren() const
{
    return false;
}

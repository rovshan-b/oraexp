#include "dbtabletriggerparentitem.h"
#include "../trigger/dbtriggeritem.h"
#include "connectivity/resultset.h"

DbTableTriggerParentItem::DbTableTriggerParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTriggerParentItem(itemText, itemName, model, parent)
{
}

QList<DbTreeItem*> DbTableTriggerParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_table_triggers"),
                                   new Param(":owner", schemaName()),
                                   new Param(":table_name", itemName()));
}

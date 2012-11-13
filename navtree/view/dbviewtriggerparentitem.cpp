#include "dbviewtriggerparentitem.h"

DbViewTriggerParentItem::DbViewTriggerParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QList<DbTreeItem*> DbViewTriggerParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_view_triggers"),
                                   new Param(":owner", schemaName()),
                                   new Param(":object_name", itemName()));
}

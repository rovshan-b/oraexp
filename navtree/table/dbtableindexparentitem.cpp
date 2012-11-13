#include "dbtableindexparentitem.h"

DbTableIndexParentItem::DbTableIndexParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbIndexParentItem(itemText, itemName, model, parent)
{
}

QList<DbTreeItem*> DbTableIndexParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_table_indexes"),
                                   new Param(":owner", schemaName()),
                                   new Param(":object_name", itemName()));
}

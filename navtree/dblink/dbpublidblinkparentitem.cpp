#include "dbpublicdblinkparentitem.h"

DbPublicDbLinkParentItem::DbPublicDbLinkParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbDbLinkParentItem(itemText, itemName, model, parent)
{
}

QList<DbTreeItem*> DbPublicDbLinkParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_dblink_list"),
                                   new Param(":owner", QString("PUBLIC")));
}

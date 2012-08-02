#include "dbpublicsynonymparentitem.h"

DbPublicSynonymParentItem::DbPublicSynonymParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbSynonymParentItem(itemText, itemName, model, parent)
{
}

QList<DbTreeItem*> DbPublicSynonymParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_synonym_list"),
                                   new Param(":owner", QString("PUBLIC")));
}

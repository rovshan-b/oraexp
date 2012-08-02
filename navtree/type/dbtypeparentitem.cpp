#include "dbtypeparentitem.h"
#include "connectivity/resultset.h"
#include "dbtypeitem.h"

#include <QList>


DbTypeParentItem::DbTypeParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbTypeParentItem::iconName() const
{
    return "type";
}

QList<DbTreeItem*> DbTypeParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_type_list"),
                                   new Param(":owner", schemaName()));
}

DbTreeItem *DbTypeParentItem::createNodeFromRecord(Resultset *rs)
{
    QString typeName=rs->getString(1);
    QString status=rs->getString(2);
    DbTypeItem *col = new DbTypeItem(typeName, typeName, this->getModel(), this);

    col->setInactive(status!="VALID");

    return col;
}

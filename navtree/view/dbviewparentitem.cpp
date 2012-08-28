#include "dbviewparentitem.h"
#include "connectivity/resultset.h"
#include "dbviewitem.h"
#include "../nodeaction.h"
#include "connection_page/dbuimanager.h"
#include "util/iconutil.h"

#include <QList>


DbViewParentItem::DbViewParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbViewParentItem::iconName() const
{
    return "view";
}

QList<DbTreeItem*> DbViewParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_view_list"), new Param(":owner", schemaName()), new Param(":view_name", nameFilter));
}

DbTreeItem *DbViewParentItem::createNodeFromRecord(Resultset *rs)
{
    QString viewName=rs->getString(1);
    QString status=rs->getString(2);
    DbViewItem *col = new DbViewItem(viewName, viewName, this->getModel(), this);

    QString tooltip=QString("Status: %1").arg(status);
    col->setTooltip(tooltip);
    col->setInvalid(status!="VALID");

    return col;
}

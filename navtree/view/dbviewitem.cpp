#include "dbviewitem.h"
#include "util/strutil.h"
#include "../table/dbtablecolumnparentitem.h"
#include "../table/dbtabletriggerparentitem.h"


DbViewItem::DbViewItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbViewItem::iconName() const
{
    return "view";
}

QList<DbTreeItem*> DbViewItem::populateChildren()
{
    QList<DbTreeItem*> children;

    children.append(new DbTableColumnParentItem(QObject::tr("Columns"), itemName(), getModel(), this));
    children.append(new DbTableTriggerParentItem(QObject::tr("Triggers"), itemName(), getModel(), this));

    return children;
}

bool DbViewItem::displayChildCount() const
{
    return false;
}

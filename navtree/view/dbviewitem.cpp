#include "dbviewitem.h"
#include "util/strutil.h"
#include "../table/dbtablecolumnparentitem.h"
#include "../table/dbtabletriggerparentitem.h"
#include "../nodeaction.h"
#include "connection_page/dbuimanager.h"
#include "util/iconutil.h"


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

QList<QAction*> DbViewItem::getContextMenuItems(const QModelIndex &index) const
{
    QList<QAction*> actions;

    actions.append(DbTreeItem::getContextMenuItems(index));

    QAction *separator=new QAction(0);
    separator->setSeparator(true);
    actions.append(separator);

    //alter
    NodeAction *alterViewAction=new NodeAction(index, IconUtil::getIcon("view_alter"), QObject::tr("Alter"));
    QObject::connect(alterViewAction, SIGNAL(triggered()), getModel()->getUiManager(), SLOT(alterView()));
    actions.append(alterViewAction);

    return actions;
}

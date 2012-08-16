#include "dbprocedureitem.h"
#include "util/strutil.h"
#include "../nodeaction.h"
#include "connection_page/dbuimanager.h"
#include "util/iconutil.h"

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

QList<QAction *> DbProcedureItem::getContextMenuItems(const QModelIndex &index) const
{
    QList<QAction*> actions;

    actions.append(DbTreeItem::getContextMenuItems(index));

    QAction *separator=new QAction(0);
    separator->setSeparator(true);
    actions.append(separator);

    //alter
    NodeAction *alterProcedureAction=new NodeAction(index, IconUtil::getIcon("procedure"), QObject::tr("Alter"));
    QObject::connect(alterProcedureAction, SIGNAL(triggered()), getModel()->getUiManager(), SLOT(alterProcedure()));
    actions.append(alterProcedureAction);

    return actions;
}

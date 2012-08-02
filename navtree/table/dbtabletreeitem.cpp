#include "dbtabletreeitem.h"
#include "util/strutil.h"
#include "dbtablecolumnparentitem.h"
#include "dbtableconstraintparentitem.h"
#include "dbtableindexparentitem.h"
#include "dbtabletriggerparentitem.h"
#include "../nodeaction.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"
#include "../dbtreemodel.h"


DbTableTreeItem::DbTableTreeItem(const QString &itemText, const QString &itemName,
                                 DbTreeModel *model, DbTreeItem *parent,
                                 OraExp::TableType tableType, bool isPartitioned) :
    DbTreeItem(itemText, itemName, model, parent),
    tableType(tableType),
    isPartitioned(isPartitioned)
{
}

QString DbTableTreeItem::iconName() const
{
    QString name="table";

    switch(tableType){
    case OraExp::TableTypeHeap:
        break;
    case OraExp::TableTypeTemporarySession:
    case OraExp::TableTypeTemporaryTransaction:
        name="table_temporary";
        break;
    case OraExp::TableTypeIndexOrganized:
    case OraExp::TableTypeIndexOrganizedMapping:
    case OraExp::TableTypeIndexOrganizedOverflow:
        name="table_index_organized";
        break;
    case OraExp::TableTypeExternal:
        name="table_external";
        break;
    }

    if(isPartitioned){
        name.append("_partitioned");
    }

    return name;
}

QList<DbTreeItem*> DbTableTreeItem::populateChildren()
{
    QList<DbTreeItem*> children;

    children.append(new DbTableColumnParentItem(QObject::tr("Columns"), itemName(), getModel(), this));
    children.append(new DbTableConstraintParentItem(QObject::tr("Constraints"), itemName(), getModel(), this));
    children.append(new DbTableIndexParentItem(QObject::tr("Indexes"), itemName(), getModel(), this));
    children.append(new DbTableTriggerParentItem(QObject::tr("Triggers"), itemName(), getModel(), this));

    return children;
}

bool DbTableTreeItem::displayChildCount() const
{
    return false;
}

QList<QAction*> DbTableTreeItem::getContextMenuItems(const QModelIndex &index) const
{
    QList<QAction*> actions;

    actions.append(DbTreeItem::getContextMenuItems(index));

    QAction *separator=new QAction(0);
    separator->setSeparator(true);
    actions.append(separator);

    //view
    NodeAction *viewDetailsAction=new NodeAction(index, IconUtil::getIcon("view_detailed"), QObject::tr("View"));
    QObject::connect(viewDetailsAction, SIGNAL(triggered()), getModel()->getUiManager(), SLOT(viewTableDetails()));
    actions.append(viewDetailsAction);

    //alter
    NodeAction *alterTableAction=new NodeAction(index, IconUtil::getIcon("table_alter"), QObject::tr("Alter"));
    QObject::connect(alterTableAction, SIGNAL(triggered()), getModel()->getUiManager(), SLOT(alterTable()));
    actions.append(alterTableAction);

    return actions;
}

DbTreeItem::CheckBehavior DbTableTreeItem::checkBehavior() const
{
    if(this->parent()!=0 && this->parent()->getItemType()==DbTreeModel::Tables){
        return DbTreeItem::Checkable;
    }else{
        return DbTreeItem::NotCheckable;
    }
}

bool DbTableTreeItem::canGenerateDdlForItem() const
{
    return tableType!=OraExp::TableTypeIndexOrganizedMapping &&
           tableType!=OraExp::TableTypeIndexOrganizedOverflow;
}

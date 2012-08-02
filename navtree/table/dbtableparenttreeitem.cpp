#include "dbtableparenttreeitem.h"
#include "connectivity/dbconnection.h"
#include "connectivity/resultset.h"
#include "dbtabletreeitem.h"
#include "../nodeaction.h"
#include "connection_page/dbuimanager.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "../dbtreemodel.h"

#include <QList>

DbTableParentTreeItem::DbTableParentTreeItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbTableParentTreeItem::iconName() const
{
    return "table";
}

QList<DbTreeItem*> DbTableParentTreeItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_table_list"), new Param(":owner", schemaName()), new Param(":table_name", nameFilter));
}

DbTreeItem *DbTableParentTreeItem::createNodeFromRecord(Resultset *rs)
{
    QString tableName=rs->getString(1);
    QString temporary=rs->getString(2);
    QString temporaryDuration=rs->getString(3);
    QString iotType=rs->getString(4);
    QString external=rs->getString(5);
    QString partitioned=rs->getString(6);

    return new DbTableTreeItem(tableName, tableName, this->getModel(), this,
                               DbUtil::getTableType(temporary, temporaryDuration, iotType, external),
                               partitioned=="YES"
                               );
}

QList<QAction*> DbTableParentTreeItem::getContextMenuItems(const QModelIndex &index) const
{
    QList<QAction*> actions;

    actions.append(DbTreeItem::getContextMenuItems(index));

    QAction *separator=new QAction(0);
    separator->setSeparator(true);
    actions.append(separator);

    NodeAction *createNewTableAction=new NodeAction(index, IconUtil::getIcon("table_add"),
                                                 QObject::tr("Create table"));
    QObject::connect(createNewTableAction, SIGNAL(triggered()),
                     getModel()->getUiManager(),
                     SLOT(showTableCreator()));
    actions.append(createNewTableAction);

    return actions;
}

QList<DbTreeItem*> DbTableParentTreeItem::createFilteredItem(const QString &sourceObjectOwner,
                                                             const QString &tableOwner,
                                                             const QString &tableName,
                                                             DbTreeModel *model,
                                                             DbTreeItem *parent)
{
    DbTableParentTreeItem tableParentItem(tableName, tableName, model, parent);
    tableParentItem.setSchemaName(tableOwner);
    if(tableOwner!=sourceObjectOwner){
        tableParentItem.setItemText(QString("%1.%2").arg(tableOwner, tableName));
    }
    tableParentItem.setNameFilter(tableName);
    return tableParentItem.populateChildren();
}

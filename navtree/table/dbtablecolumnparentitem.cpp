#include "dbtablecolumnparentitem.h"
#include "dbtablecolumnitem.h"
#include "util/strutil.h"
#include <iostream>
#include <QList>
#include "connectivity/resultset.h"
#include <memory>


DbTableColumnParentItem::DbTableColumnParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model,
                                                 DbTreeItem *parent) : DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbTableColumnParentItem::iconName() const
{
    return "column";
}

QList<DbTreeItem*> DbTableColumnParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_table_columns"),
                                   new Param(":owner", schemaName()),
                                   new Param(":object_name", itemName()));
}

DbTreeItem *DbTableColumnParentItem::createNodeFromRecord(Resultset *rs)
{
    QString columnName=rs->getString(1);
    QString dataType=rs->getString(2);
    QString tooltip=QObject::tr("%1 - %2(%3)\nNullable: %4\nUnique: %5").arg(columnName, dataType).
                                       arg(rs->getInt(3)).
                                       arg(rs->getString(4)).
                                       arg(rs->getInt(8)==1 ? 'Y' : 'N');
    DbTableColumnItem *col = new DbTableColumnItem(columnName, columnName, this->getModel(), dataType, this);
    col->setTooltip(tooltip);
    col->setPrimaryKey(rs->getInt(5)==1);
    col->setForeignKey(rs->getInt(6)==1);
    col->setCheckConstraint(rs->getInt(7)==1);
    col->setUnique(rs->getInt(8)==1);
    return col;
}

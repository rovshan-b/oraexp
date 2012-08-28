#include "dbtableconstraintparentitem.h"
#include "dbtableconstraintitem.h"
#include "connectivity/resultset.h"

#include <memory>


DbTableConstraintParentItem::DbTableConstraintParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model,
                                                 DbTreeItem *parent) : DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbTableConstraintParentItem::iconName() const
{
    return "constraint";
}

QList<DbTreeItem*> DbTableConstraintParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_table_constraints"),
                                   new Param(":owner", schemaName()),
                                   new Param(":table_name", itemName()));
}

DbTreeItem *DbTableConstraintParentItem::createNodeFromRecord(Resultset *rs)
{
    QString constraintName=rs->getString(1);
    QString constraintType=rs->getString(2);
    QString searchCondition=rs->getString(3);
    QString deleteRule=rs->getString(4);
    QString status=rs->getString(5);

    QString tooltip=QObject::tr("Status: %1").
                    arg(status);

    if(constraintType=="R"){
        tooltip.append("\n").append(QObject::tr("Delete rule: %1").arg(deleteRule));
    }

    if(constraintType=="C"){
        tooltip.append("\n").append(QObject::tr("Search condition: %1").arg(searchCondition));
    }

    DbTableConstraintItem *col=new DbTableConstraintItem(constraintName, constraintName, getModel(), this);
    col->setTooltip(tooltip);
    col->setConstraintType(constraintType);
    col->setInvalid(status!="ENABLED");

    return col;
}

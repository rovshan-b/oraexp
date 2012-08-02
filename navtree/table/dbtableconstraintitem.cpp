#include "dbtableconstraintitem.h"
#include "dbconstraintcolumnitem.h"

#include "connectivity/resultset.h"

DbTableConstraintItem::DbTableConstraintItem(const QString &itemText, const QString &itemName, DbTreeModel *model,
                                                 DbTreeItem *parent) : DbTreeItem(itemText, itemName, model, parent)
{
}


QString DbTableConstraintItem::iconName() const
{
    QString iconName="constraint";
    if(constraintType=="P"){
        iconName.append("_pk");
    }else if(constraintType=="R"){
        iconName.append("_fk");
    }else if(constraintType=="U"){
        iconName.append("_uq");
    }else if(constraintType=="C"){
        iconName.append("_ch");
    }

    return iconName;
}

void DbTableConstraintItem::setConstraintType(const QString &constraintType)
{
    this->constraintType=constraintType;
}

QList<DbTreeItem*> DbTableConstraintItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_constraint_columns"),
                                   new Param(":owner", schemaName()),
                                   new Param(":constraint_name", itemName()));
}

DbTreeItem *DbTableConstraintItem::createNodeFromRecord(Resultset *rs)
{
    QString tableName=rs->getString(1);
    QString columnName=rs->getString(2);

    DbConstraintColumnItem *col=new DbConstraintColumnItem(columnName,
                                                           columnName, tableName, getModel(), this);
    col->setConstraintType(constraintType);

    return col;
}

bool DbTableConstraintItem::displayChildCount() const
{
    return false;
}

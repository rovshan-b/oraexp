#include "dbconstraintcolumnitem.h"
#include "dbtableconstraintitem.h"
#include "dbtabletreeitem.h"
#include "dbtableparenttreeitem.h"
#include "util/dbutil.h"
#include <memory>
#include "connectivity/resultset.h"


DbConstraintColumnItem::DbConstraintColumnItem(const QString &itemText, const QString &itemName, const QString &tableName, DbTreeModel *model,
                                                 DbTreeItem *parent) : DbTreeItem(itemText, itemName, model, parent)
{
    this->tableName=tableName;
}

QString DbConstraintColumnItem::iconName() const
{
    return "column";
}

bool DbConstraintColumnItem::hasChildren() const
{
    //return true for foreign key constraints
    return (constraintType=="R");
}

void DbConstraintColumnItem::setConstraintType(const QString &constraintType)
{
    this->constraintType=constraintType;
}

//will be called for foreign key constraints
QList<DbTreeItem*> DbConstraintColumnItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_column_constraints"),
                                   new Param(":owner", schemaName()),
                                   new Param(":table_name", this->tableName),
                                   new Param(":column_name", itemName()));
}


DbTreeItem *DbConstraintColumnItem::createNodeFromRecord(Resultset *rs)
{
    QString schemaName=rs->getString(1);
    QString tableName=rs->getString(2);
    QString temporary=rs->getString(3);
    QString temporaryDuration=rs->getString(4);
    QString iotType=rs->getString(5);
    QString external=rs->getString(6);
    QString partitioned=rs->getString(7);

    DbTableTreeItem *newItem=new DbTableTreeItem(tableName, tableName, this->getModel(), this,
                                                 DbUtil::getTableType(temporary, temporaryDuration, iotType, external),
                                                 partitioned=="YES"
                                                 );
    if(this->schemaName()!=schemaName){
        newItem->setSchemaName(schemaName);
        newItem->setItemText(QString("%1.%2").arg(schemaName, tableName));
    }

    return newItem;
}

bool DbConstraintColumnItem::displayChildCount() const
{
    return false;
}

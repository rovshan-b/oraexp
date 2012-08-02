#include "dbtablecolumnitem.h"
#include "dbtabletreeitem.h"
#include "dbtableparenttreeitem.h"
#include "util/param.h"
#include "util/strutil.h"
#include "util/dbutil.h"
#include "connectivity/resultset.h"


DbTableColumnItem::DbTableColumnItem(const QString &itemText, const QString &itemName, DbTreeModel *model,
                                                 const QString &dataType, DbTreeItem *parent) :
                                                 DbTreeItem(itemText, itemName, model, parent),
                                                 m_dataType(dataType),
                                                 isPrimaryKey(false),
                                                 isForeignKey(false),
                                                 hasCheckConstraint(false),
                                                 isUnique(false)
{
}

QString DbTableColumnItem::iconName() const
{
    QString iconName = "column";
    if(isPrimaryKey){
        iconName="column_pk";
    }else if(isForeignKey && isUnique){
        iconName="column_fk_uq";
    }else if(isForeignKey){
        iconName="column_fk";
    }else if(isUnique){
        iconName="column_uq";
    }

    return iconName;
}

bool DbTableColumnItem::hasChildren() const
{
    return isForeignKey;
}

QList<DbTreeItem*> DbTableColumnItem::populateChildren() //will be called only for foreign key columns
{
    return createNodeForEachRecord(getQuery("get_column_constraints"),
                                   new Param(":owner", schemaName()),
                                   new Param(":table_name", this->parent()->itemName()),
                                   new Param(":column_name", itemName()));
}

QString DbTableColumnItem::dataType() const
{
    return m_dataType;
}

DbTreeItem *DbTableColumnItem::createNodeFromRecord(Resultset *rs)
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
    newItem->setIsTopLevel(false);

    if(this->schemaName()!=schemaName){
        newItem->setSchemaName(schemaName);
        newItem->setItemText(QString("%1.%2").arg(schemaName, tableName));
    }

    return newItem;
}

void DbTableColumnItem::setPrimaryKey(bool isPrimaryKey)
{
    this->isPrimaryKey=isPrimaryKey;
}

void DbTableColumnItem::setForeignKey(bool isForeignKey)
{
    this->isForeignKey=isForeignKey;
}

void DbTableColumnItem::setCheckConstraint(bool hasCheckConstraint)
{
    this->hasCheckConstraint=hasCheckConstraint;
}

void DbTableColumnItem::setUnique(bool isUnique)
{
    this->isUnique=isUnique;
}


bool DbTableColumnItem::displayChildCount() const
{
    return false;
}

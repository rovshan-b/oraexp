#include "dbindexparentitem.h"
#include "dbindexitem.h"

#include "connectivity/resultset.h"

DbIndexParentItem::DbIndexParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
    DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbIndexParentItem::iconName() const
{
    return "index";
}

QList<DbTreeItem*> DbIndexParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_index_list"), new Param(":owner", schemaName()));
}

DbTreeItem *DbIndexParentItem::createNodeFromRecord(Resultset *rs)
{
    //owner, index_name, index_type, uniqueness, status
    QString owner=rs->getString(1);
    QString indexName=rs->getString(2);
    QString indexType=rs->getString(3);
    QString uniqueness=rs->getString(4);
    QString status=rs->getString(5);
    QString tableOwner=rs->getString(6);
    QString tableName=rs->getString(7);

    DbIndexItem *col = new DbIndexItem(indexName, indexName, this->getModel(), this);
    col->setSchemaName(owner);

    if(this->schemaName()!=owner){
        col->setItemText(QString("%1.%2").arg(owner, indexName));
    }

    QString tooltip = QObject::tr("Type: %1\nUniqueness: %2\nStatus: %3\nTable owner: %4\nTable name: %5").
                      arg(indexType).
                      arg(uniqueness).
                      arg(status).
                      arg(tableOwner).
                      arg(tableName);

    col->setTooltip(tooltip);
    col->setUnique(uniqueness=="UNIQUE");
    col->setInvalid(status!="VALID");

    return col;
}

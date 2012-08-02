#include "dbsynonymparentitem.h"
#include "connectivity/resultset.h"
#include "dbsynonymitem.h"
#include "dbpublicsynonymparentitem.h"

#include <QList>


DbSynonymParentItem::DbSynonymParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbSynonymParentItem::iconName() const
{
    return "synonym";
}

QList<DbTreeItem*> DbSynonymParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_synonym_list"),
                                   new Param(":owner", schemaName()));
}

DbTreeItem *DbSynonymParentItem::createNodeFromRecord(Resultset *rs)
{
    //object_name, table_owner, table_name, db_link, status
    QString synonymName=rs->getString(1);
    QString tableOwner=rs->getString(2);
    QString tableName=rs->getString(3);
    QString dbLink=rs->getString(4);
    QString status=rs->getString(5);

    DbSynonymItem *col = new DbSynonymItem(synonymName, synonymName, this->getModel(), this);

    col->setTooltip(QObject::tr("Table owner: %1\n"
                                "Table name: %2\n"
                                "Database link: %3").
                    arg(tableOwner).
                    arg(tableName).
                    arg(dbLink));

    col->setInactive(status!="VALID");

    return col;
}

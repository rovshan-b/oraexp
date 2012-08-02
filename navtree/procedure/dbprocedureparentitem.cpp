#include "dbprocedureparentitem.h"
#include "connectivity/resultset.h"
#include "dbprocedureitem.h"

#include <QList>


DbProcedureParentItem::DbProcedureParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbProcedureParentItem::iconName() const
{
    return "procedure";
}

QList<DbTreeItem*> DbProcedureParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_procedure_list"),
                                   new Param(":owner", schemaName()));
}

DbTreeItem *DbProcedureParentItem::createNodeFromRecord(Resultset *rs)
{
    QString procedureName=rs->getString(1);
    QString status=rs->getString(2);
    DbProcedureItem *col = new DbProcedureItem(procedureName, procedureName, this->getModel(), this);

    col->setInactive(status!="VALID");

    return col;
}

#include "dbfunctionparentitem.h"
#include "connectivity/resultset.h"
#include "dbfunctionitem.h"

#include <QList>


DbFunctionParentItem::DbFunctionParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbFunctionParentItem::iconName() const
{
    return "function";
}

QList<DbTreeItem*> DbFunctionParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_function_list"),
                                   new Param(":owner", schemaName()));
}

DbTreeItem *DbFunctionParentItem::createNodeFromRecord(Resultset *rs)
{
    QString functionName=rs->getString(1);
    QString status=rs->getString(2);
    DbFunctionItem *col = new DbFunctionItem(functionName, functionName, this->getModel(), this);

    col->setInvalid(status!="VALID");

    return col;
}

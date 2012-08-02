#include "dballschemasitem.h"
#include "dbschemaitem.h"
#include "connectivity/resultset.h"

DbAllSchemasItem::DbAllSchemasItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent,
                                   DbTreeModel::DbTreeNodeTypes nodeTypesToDisplay) :
    DbTreeItem(itemText, itemName, model, parent), nodeTypesToDisplay(nodeTypesToDisplay)
{
}

QString DbAllSchemasItem::iconName() const
{
    return "users";
}

QList<DbTreeItem*> DbAllSchemasItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_schema_list"));
}

DbTreeItem *DbAllSchemasItem::createNodeFromRecord(Resultset *rs)
{
    QString schemaName=rs->getString(1);
    DbSchemaItem *col = new DbSchemaItem(schemaName, schemaName, this->getModel(), this);
    col->setNodeTypesToDisplay(nodeTypesToDisplay);
    col->setSchemaName(schemaName);

    return col;
}

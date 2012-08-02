#include "dbindexcolumnitem.h"
#include "../table/dbtableparenttreeitem.h"
DbIndexColumnItem::DbIndexColumnItem(const QString &itemText, const QString &itemName, DbTreeModel *model,
                                     const QString &dataType,
                                     const QString &schemaName,
                                     const QString &tableOwner, const QString &tableName,
                                     bool showChildren, DbTreeItem *parent) :
    DbTableColumnItem(itemText, itemName, model, dataType, parent), tableOwner(tableOwner), tableName(tableName), showChildren(showChildren)
{
    bool isUnderSchemaIndexesNode=false;
    if(this->parent()){
        if(this->parent()->parent() && this->parent()->parent()->getItemType()==DbTreeModel::SchemaIndexes){
            isUnderSchemaIndexesNode=true;
        }
    }

    QString newText=isUnderSchemaIndexesNode ? QString("%1.%2").arg(tableName, itemText) : itemText;
    if(schemaName!=tableOwner){
        newText.prepend(QString("%1.").arg(tableOwner));
    }

    setItemText(newText);
}

bool DbIndexColumnItem::hasChildren() const
{
    return showChildren;
}

QList<DbTreeItem*> DbIndexColumnItem::populateChildren()
{
    return DbTableParentTreeItem::createFilteredItem(schemaName(), tableOwner, tableName, getModel(), this);
}


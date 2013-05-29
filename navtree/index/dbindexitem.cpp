#include "dbindexitem.h"
#include "dbindexcolumnitem.h"

#include "util/param.h"
#include "connectivity/resultset.h"

DbIndexItem::DbIndexItem(const QString &itemText, const QString &itemName, DbTreeModel *model,
                                                 DbTreeItem *parent) :
                                        DbTreeItem(itemText, itemName, model, parent),
                                        isUnique(false)
{
}

QString DbIndexItem::iconName() const
{
    QString iconName = "index";
    if(isUnique){
        iconName.append("_uq");
    }

    return iconName;
}

void DbIndexItem::setUnique(bool unique)
{
    this->isUnique=unique;
}


QList<DbTreeItem*> DbIndexItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_index_columns"),
                                   new Param(":owner", schemaName()),
                                   new Param(":index_name", itemName()));
}

QString DbIndexItem::getParentItemNameForContextMenu() const
{
    return ""; //always return empty string. table creator will find out by itself
}

DbTreeItem *DbIndexItem::createNodeFromRecord(Resultset *rs)
{
    QString columnName=rs->getString(1);
    QString tableOwner=rs->getString(2);
    QString tableName=rs->getString(3);
    QString dataType=rs->getString(4);

    //DbGenericLeafItem *newItem=new DbGenericLeafItem(columnName, columnName, "column", this->getModel(), this);
    bool showChildren=true;
    if(this->parent()){
        if(this->parent()->parent()){
            if(this->parent()->parent()->getItemType()==DbTreeModel::Table){
                showChildren=false;
            }
        }
    }
    DbIndexColumnItem *newItem=new DbIndexColumnItem(columnName, columnName,
                                                     this->getModel(), dataType,
                                                     schemaName(), tableOwner,
                                                     tableName, showChildren, this);

    return newItem;
}

bool DbIndexItem::displayChildCount() const
{
    return false;
}

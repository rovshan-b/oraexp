#include "dbtriggeritem.h"
#include "connectivity/resultset.h"
#include "../schema/dbschemaitem.h"
#include "../table/dbtableparenttreeitem.h"
#include "../view/dbviewparentitem.h"

DbTriggerItem::DbTriggerItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbTriggerItem::iconName() const
{
    return "trigger";
}

bool DbTriggerItem::hasChildren() const
{
    return this->parent()->getItemType()==DbTreeModel::SchemaTriggers &&
            (baseObjectType=="SCHEMA" || baseObjectType=="TABLE" || baseObjectType=="VIEW");
}

bool DbTriggerItem::displayChildCount() const
{
    return false;
}

QList<DbTreeItem*> DbTriggerItem::populateChildren()
{
    Q_ASSERT(baseObjectType=="SCHEMA" || baseObjectType=="TABLE" || baseObjectType=="VIEW");

    if(baseObjectType=="SCHEMA"){
        DbSchemaItem *schemaItem=new DbSchemaItem(baseObjectOwner, baseObjectOwner, getModel(), this);
        schemaItem->setSchemaName(baseObjectOwner);
        return QList<DbTreeItem*>() << schemaItem;
    }else if(baseObjectType=="TABLE"){       
        return DbTableParentTreeItem::createFilteredItem(schemaName(), baseObjectOwner, baseObjectName, getModel(), this);
    }else{
        DbViewParentItem viewParentItem(baseObjectName, baseObjectName, getModel(), this);
        viewParentItem.setSchemaName(baseObjectOwner);
        if(baseObjectOwner!=schemaName()){
            viewParentItem.setItemText(QString("%1.%2").arg(baseObjectOwner, baseObjectName));
        }
        viewParentItem.setNameFilter(baseObjectName);
        return viewParentItem.populateChildren();
    }
}

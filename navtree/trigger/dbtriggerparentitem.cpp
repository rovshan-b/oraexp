#include "dbtriggerparentitem.h"
#include "dbtriggeritem.h"
#include "connectivity/resultset.h"

DbTriggerParentItem::DbTriggerParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
    DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbTriggerParentItem::iconName() const
{
    return "trigger";
}

QList<DbTreeItem*> DbTriggerParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_trigger_list"),
                                   new Param(":owner", schemaName()));
}

DbTreeItem *DbTriggerParentItem::createNodeFromRecord(Resultset *rs)
{
    //owner, trigger_name, trigger_type, triggering_event, status
    QString owner=rs->getString(1);
    QString triggerName=rs->getString(2);
    QString triggerType=rs->getString(3);
    QString triggeringEvent=rs->getString(4);
    QString status=rs->getString(5);
    QString baseObjectType=rs->getString(6);
    QString baseObjectOwner=rs->getString(7);
    QString baseObjectName=rs->getString(8);

    DbTriggerItem *col = new DbTriggerItem(triggerName, triggerName, this->getModel(), this);
    col->setSchemaName(owner);
    col->setBaseObjectType(baseObjectType);
    col->setBaseObjectOwner(baseObjectOwner);
    col->setBaseObjectName(baseObjectName);

    if(this->schemaName()!=owner){
        col->setItemText(QString("%1.%2").arg(owner, triggerName));
    }

    QString tooltip = QObject::tr("Event: %1\nType: %2\nStatus %3\nBase object type %4\nBase object name %5").
                      arg(triggeringEvent).
                      arg(triggerType).
                      arg(status).
                      arg(baseObjectType).
                      arg(baseObjectName.isEmpty() ? baseObjectOwner : baseObjectName);
    col->setTooltip(tooltip);
    col->setInvalid(status!="ENABLED");

    return col;
}

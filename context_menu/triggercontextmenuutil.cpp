#include "triggercontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

TriggerContextMenuUtil::TriggerContextMenuUtil()
{
}

QList<QAction *> TriggerContextMenuUtil::getActionsForObject(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::SchemaTriggers || itemType==DbTreeModel::Triggers){
        QAction *createTriggerAction=new DbItemAction(IconUtil::getIcon("trigger_add"), QObject::tr("Create trigger..."),
                                                       schemaName, "", itemType,
                                                       uiManager, SLOT(createCreator()));
        results.append(createTriggerAction);
    }else if(itemType==DbTreeModel::Trigger){
        //view
        QAction *viewTriggerAction=new DbItemAction(IconUtil::getIcon("trigger_view"), QObject::tr("View"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createViewer()));

        //alter
        DbItemAction *alterTriggerAction=new DbItemAction(IconUtil::getIcon("trigger_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        alterTriggerAction->isDefault = true;
        results.append(viewTriggerAction);
        results.append(alterTriggerAction);
    }

    return results;
}

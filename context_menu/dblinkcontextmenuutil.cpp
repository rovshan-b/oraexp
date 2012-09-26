#include "dblinkcontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

DbLinkContextMenuUtil::DbLinkContextMenuUtil()
{
}

QList<QAction *> DbLinkContextMenuUtil::getActionsForObject(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::DatabaseLinks){
        DbItemAction *createDbLinkAction=new DbItemAction(IconUtil::getIcon("dblink_add"), QObject::tr("Create database link"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createCreator()));
        if(schemaName=="PUBLIC"){
            createDbLinkAction->properties["public"]="yes";
        }
        results.append(createDbLinkAction);
    }else if(itemType==DbTreeModel::DatabaseLink){
        //alter
        QAction *alterDbLinkAction=new DbItemAction(IconUtil::getIcon("dblink_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        results.append(alterDbLinkAction);
    }

    return results;
}

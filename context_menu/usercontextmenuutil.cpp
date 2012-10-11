#include "usercontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

UserContextMenuUtil::UserContextMenuUtil()
{
}

QList<QAction *> UserContextMenuUtil::getActionsForObject(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::AllSchemas){
        QAction *createUserAction=new DbItemAction(IconUtil::getIcon("user_add"), QObject::tr("Create schema"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createCreator()));
        results.append(createUserAction);
    }else if(itemType==DbTreeModel::Schema){
        //alter
        QAction *alterUserAction=new DbItemAction(IconUtil::getIcon("user_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        results.append(alterUserAction);
    }

    return results;
}

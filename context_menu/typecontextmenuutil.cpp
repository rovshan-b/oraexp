#include "typecontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

TypeContextMenuUtil::TypeContextMenuUtil()
{
}

QList<QAction *> TypeContextMenuUtil::getActionsForObject(const QString &schemaName, const QString &objectName,
                                                             const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::Types){
        QAction *createTypeAction=new DbItemAction(IconUtil::getIcon("type_add"), QObject::tr("Create type"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createCreator()));
        results.append(createTypeAction);
    }else if(itemType==DbTreeModel::Type ||
             itemType==DbTreeModel::TypeSpec ||
             itemType==DbTreeModel::TypeBody){
        //alter
        QAction *alterTypeAction=new DbItemAction(IconUtil::getIcon("type_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        results.append(alterTypeAction);
    }

    return results;
}

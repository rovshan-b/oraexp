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
    }else if(itemType==DbTreeModel::TypeSpec ||
             itemType==DbTreeModel::TypeBody){
        //view
        QAction *viewTypeAction=new DbItemAction(IconUtil::getIcon("type_view"), QObject::tr("View"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createViewer()));

        //alter
        DbItemAction *alterTypeAction=new DbItemAction(IconUtil::getIcon("type_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        alterTypeAction->isDefault = true;
        results.append(viewTypeAction);
        results.append(alterTypeAction);
    }else if(itemType==DbTreeModel::Type){

        //view
        QAction *viewSpecAction=new DbItemAction(IconUtil::getIcon("type_view"), QObject::tr("View"),
                                                       schemaName, objectName, DbTreeModel::TypeSpec,
                                                       uiManager, SLOT(createViewer()));

        //alter
        DbItemAction *alterSpecAction=new DbItemAction(IconUtil::getIcon("type_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, DbTreeModel::TypeSpec,
                                                       uiManager, SLOT(createEditor()));
        alterSpecAction->isDefault = true;

        results.append(viewSpecAction);
        results.append(alterSpecAction);
    }

    return results;
}

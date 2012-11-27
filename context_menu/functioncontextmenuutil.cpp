#include "functioncontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

FunctionContextMenuUtil::FunctionContextMenuUtil()
{
}

QList<QAction *> FunctionContextMenuUtil::getActionsForObject(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::Functions){
        QAction *createFunctionAction=new DbItemAction(IconUtil::getIcon("function_add"), QObject::tr("Create function"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createCreator()));
        results.append(createFunctionAction);
    }else if(itemType==DbTreeModel::Function){
        //view
        QAction *viewFunctionAction=new DbItemAction(IconUtil::getIcon("function_view"), QObject::tr("View"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createViewer()));

        //alter
        QAction *alterFunctionAction=new DbItemAction(IconUtil::getIcon("function_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));

        results.append(viewFunctionAction);
        results.append(alterFunctionAction);
    }

    return results;
}

#include "viewcontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

ViewContextMenuUtil::ViewContextMenuUtil()
{
}

QList<QAction *> ViewContextMenuUtil::getActionsForObject(const QString &schemaName,
                                                          const QString &objectName,
                                                          const DbTreeModel::DbTreeNodeType itemType,
                                                          DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::Views){

        QAction *createNewViewAction=new DbItemAction(IconUtil::getIcon("view_add"), QObject::tr("Create view"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createCreator()));
        results.append(createNewViewAction);

    }else if(itemType==DbTreeModel::View){
        //view
        QAction *viewDetailsAction=new DbItemAction(IconUtil::getIcon("view_view"), QObject::tr("View"),
                                                    schemaName, objectName, itemType,
                                                    uiManager, SLOT(createViewer()));
        results.append(viewDetailsAction);

        //alter
        QAction *alterViewAction=new DbItemAction(IconUtil::getIcon("view_alter"), QObject::tr("Alter"),
                                                   schemaName, objectName, itemType,
                                                   uiManager, SLOT(createEditor()));
        results.append(alterViewAction);

    }

    return results;
}

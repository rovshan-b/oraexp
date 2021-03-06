#include "indexcontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

IndexContextMenuUtil::IndexContextMenuUtil()
{
}

QList<QAction *> IndexContextMenuUtil::getActionsForObject(const QString &schemaName,
                                                           const QString &objectName,
                                                           const DbTreeModel::DbTreeNodeType itemType,
                                                           DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::Index){
        //view
        DbItemAction *viewIndexAction=new DbItemAction(IconUtil::getIcon("index_view"), QObject::tr("View"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createViewer()));
        viewIndexAction->isDefault = true;

        //alter
        /*QAction *alterIndexAction=new DbItemAction(IconUtil::getIcon("index_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));*/

        results.append(viewIndexAction);
        //results.append(alterIndexAction);
    }

    return results;
}

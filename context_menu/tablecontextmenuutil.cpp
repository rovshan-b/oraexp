#include "tablecontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

TableContextMenuUtil::TableContextMenuUtil()
{
}

QList<QAction *> TableContextMenuUtil::getActionsForObject(const QString &schemaName,
                                                           const QString &objectName,
                                                           const DbTreeModel::DbTreeNodeType itemType,
                                                           DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::Tables){

        QAction *createNewTableAction=new DbItemAction(IconUtil::getIcon("table_add"), QObject::tr("Create table"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createCreator()));
        results.append(createNewTableAction);

    }else if(itemType==DbTreeModel::Table){
        //view
        QAction *viewDetailsAction=new DbItemAction(IconUtil::getIcon("view_detailed"), QObject::tr("View"),
                                                    schemaName, objectName, itemType,
                                                    uiManager, SLOT(createViewer()));
        results.append(viewDetailsAction);

        //alter
        QAction *alterTableAction=new DbItemAction(IconUtil::getIcon("table_alter"), QObject::tr("Alter"),
                                                   schemaName, objectName, itemType,
                                                   uiManager, SLOT(createEditor()));
        results.append(alterTableAction);
    }

    return results;
}

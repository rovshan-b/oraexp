#include "tablecontextmenuutil.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
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
        QAction *viewDetailsAction=new DbItemAction(IconUtil::getIcon("table_view"), QObject::tr("View"),
                                                    schemaName, objectName, itemType,
                                                    uiManager, SLOT(createViewer()));
        results.append(viewDetailsAction);

        //alter
        QAction *alterTableAction=new DbItemAction(IconUtil::getIcon("table_alter"), QObject::tr("Alter"),
                                                   schemaName, objectName, itemType,
                                                   uiManager, SLOT(createEditor()));
        results.append(alterTableAction);

        results.append(WidgetHelper::createSeparatorAction());

        QAction *compareDataAction = new DbItemAction(IconUtil::getIcon("compare_data"), QObject::tr("Compare data..."),
                                                      schemaName, objectName, itemType,
                                                      uiManager, SLOT(addDataComparer()));
        results.append(compareDataAction);

        QAction *copyDataAction = new DbItemAction(IconUtil::getIcon("copy_data"), QObject::tr("Copy data..."),
                                                      schemaName, objectName, itemType,
                                                      uiManager, SLOT(addDataCopier()));
        results.append(copyDataAction);

        QAction *exportDataAction = new DbItemAction(IconUtil::getIcon("export"), QObject::tr("Export data..."),
                                                      schemaName, objectName, itemType,
                                                      uiManager, SLOT(addDataExporter()));
        results.append(exportDataAction);

        QAction *importDataAction = new DbItemAction(IconUtil::getIcon("import_data"), QObject::tr("Import data..."),
                                                      schemaName, objectName, itemType,
                                                      uiManager, SLOT(addDataImporter()));
        results.append(importDataAction);
    }

    return results;
}

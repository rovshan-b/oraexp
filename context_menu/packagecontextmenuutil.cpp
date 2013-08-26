#include "packagecontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

PackageContextMenuUtil::PackageContextMenuUtil()
{
}

QList<QAction *> PackageContextMenuUtil::getActionsForObject(const QString &schemaName, const QString &objectName,
                                                             const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::Packages){
        QAction *createPackageAction=new DbItemAction(IconUtil::getIcon("package_add"), QObject::tr("Create package"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createCreator()));
        results.append(createPackageAction);
    }else if(itemType==DbTreeModel::PackageSpec ||
             itemType==DbTreeModel::PackageBody){
        //view
        QAction *viewPackageAction=new DbItemAction(IconUtil::getIcon("package_view"), QObject::tr("View"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createViewer()));

        //alter
        DbItemAction *alterPackageAction=new DbItemAction(IconUtil::getIcon("package_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        alterPackageAction->isDefault = true;
        results.append(viewPackageAction);
        results.append(alterPackageAction);
    }else if(itemType==DbTreeModel::Package){
        //view
        QAction *viewSpecAction=new DbItemAction(IconUtil::getIcon("package_view"), QObject::tr("View Spec"),
                                                       schemaName, objectName, DbTreeModel::PackageSpec,
                                                       uiManager, SLOT(createViewer()));

        QAction *viewBodyAction=new DbItemAction(IconUtil::getIcon("package_view"), QObject::tr("View Body"),
                                                       schemaName, objectName, DbTreeModel::PackageBody,
                                                       uiManager, SLOT(createViewer()));


        //alter
        QAction *alterSpecAction=new DbItemAction(IconUtil::getIcon("package_alter"), QObject::tr("Alter Spec"),
                                                       schemaName, objectName, DbTreeModel::PackageSpec,
                                                       uiManager, SLOT(createEditor()));
        DbItemAction *alterBodyAction=new DbItemAction(IconUtil::getIcon("package_alter"), QObject::tr("Alter Body"),
                                                       schemaName, objectName, DbTreeModel::PackageBody,
                                                       uiManager, SLOT(createEditor()));
        alterBodyAction->isDefault = true;

        results.append(viewSpecAction);
        results.append(viewBodyAction);
        results.append(alterSpecAction);
        results.append(alterBodyAction);
    }

    return results;
}

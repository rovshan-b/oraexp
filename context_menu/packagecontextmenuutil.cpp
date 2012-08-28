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
    }else if(itemType==DbTreeModel::Package ||
             itemType==DbTreeModel::PackageSpec ||
             itemType==DbTreeModel::PackageBody){
        //alter
        QAction *alterPackageAction=new DbItemAction(IconUtil::getIcon("package_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        results.append(alterPackageAction);
    }

    return results;
}

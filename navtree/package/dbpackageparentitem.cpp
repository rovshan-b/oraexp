#include "dbpackageparentitem.h"
#include "dbpackageitem.h"
#include "connectivity/resultset.h"

DbPackageParentItem::DbPackageParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbPackageParentItem::iconName() const
{
    return "package";
}

QList<DbTreeItem*> DbPackageParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_package_list"),
                                   new Param(":owner", schemaName()));
}

DbTreeItem *DbPackageParentItem::createNodeFromRecord(Resultset *rs)
{
    //package name, status
    QString packageName=rs->getString(1);
    QString status=rs->getString(2);

    DbPackageItem *col = new DbPackageItem(packageName, packageName, this->getModel(), this);

    col->setInactive(status!="VALID");

    return col;
}


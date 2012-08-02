#include "dbpackageitem.h"
#include "connectivity/resultset.h"
#include "dbpackagespecitem.h"
#include "dbpackagebodyitem.h"

DbPackageItem::DbPackageItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbPackageItem::iconName() const
{
    return "package";
}

bool DbPackageItem::displayChildCount() const
{
    return false;
}

QList<DbTreeItem*> DbPackageItem::populateChildren()
{
    QList<DbTreeItem*> children;
    DbPackageSpecItem *specItem=new DbPackageSpecItem(QObject::tr("Spec"), itemName(), this->getModel(), this);
    specItem->setTooltip(this->tooltip());
    children.append(specItem);

    children.append(
        createNodeForEachRecord(getQuery("get_package_bodies"),
                                   new Param(":object_name", itemName()),
                                   new Param(":owner", schemaName()))
    );

    return children;
}

DbTreeItem *DbPackageItem::createNodeFromRecord(Resultset *rs)
{
    //package name, status
    QString packageName=rs->getString(1);
    QString status=rs->getString(2);

    DbPackageBodyItem *col = new DbPackageBodyItem(QObject::tr("Body"), packageName, this->getModel(), this);

    col->setInactive(status!="VALID");

    return col;
}


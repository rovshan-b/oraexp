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
    if(!isInvalid() && this->m_bodyInvalid){
        return "package_invalid";
    }else{
        return "package";
    }
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
    specItem->setInvalid(this->isInvalid());
    children.append(specItem);

    children.append(
        createNodeForEachRecord(getQuery("get_package_bodies"),
                                   new Param(":object_name", itemName()),
                                   new Param(":owner", schemaName()))
    );

    return children;
}

void DbPackageItem::setBodyInvalid(bool invalid)
{
    this->m_bodyInvalid=invalid;
}

DbTreeItem *DbPackageItem::createNodeFromRecord(Resultset *rs)
{
    //package name, status
    QString packageName=rs->getString(1);
    QString status=rs->getString(2);

    DbPackageBodyItem *col = new DbPackageBodyItem(QObject::tr("Body"), packageName, this->getModel(), this);

    col->setInvalid(status!="VALID");

    return col;
}


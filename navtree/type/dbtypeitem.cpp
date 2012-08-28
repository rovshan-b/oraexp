#include "dbtypeitem.h"
#include "connectivity/resultset.h"
#include "dbtypespecitem.h"
#include "dbtypebodyitem.h"
#include "util/strutil.h"

DbTypeItem::DbTypeItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbTypeItem::iconName() const
{
    if(!isInvalid() && this->m_bodyInvalid){
        return "type_invalid";
    }else{
        return "type";
    }
}

bool DbTypeItem::displayChildCount() const
{
    return false;
}

QList<DbTreeItem *> DbTypeItem::populateChildren()
{
    QList<DbTreeItem*> children;
    DbTypeSpecItem *specItem=new DbTypeSpecItem(QObject::tr("Spec"), itemName(), this->getModel(), this);
    specItem->setTooltip(this->tooltip());
    children.append(specItem);

    children.append(
        createNodeForEachRecord(getQuery("get_type_bodies"),
                                   new Param(":object_name", itemName()),
                                   new Param(":owner", schemaName()))
    );

    return children;
}

void DbTypeItem::setBodyInvalid(bool invalid)
{
    this->m_bodyInvalid=invalid;
}

DbTreeItem *DbTypeItem::createNodeFromRecord(Resultset *rs)
{
    QString packageName=rs->getString(1);
    QString status=rs->getString(2);

    DbTypeBodyItem *col = new DbTypeBodyItem(QObject::tr("Body"), packageName, this->getModel(), this);

    col->setInvalid(status!="VALID");

    return col;
}

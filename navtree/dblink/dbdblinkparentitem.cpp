#include "dbdblinkparentitem.h"
#include "connectivity/resultset.h"
#include "dbdblinkitem.h"

#include <QList>


DbDbLinkParentItem::DbDbLinkParentItem(const QString &itemText, const QString &itemName, DbTreeModel *model, DbTreeItem *parent) :
        DbTreeItem(itemText, itemName, model, parent)
{
}

QString DbDbLinkParentItem::iconName() const
{
    return "dblink";
}

QList<DbTreeItem*> DbDbLinkParentItem::populateChildren()
{
    return createNodeForEachRecord(getQuery("get_dblink_list"),
                                   new Param(":owner", schemaName()));
}

DbTreeItem *DbDbLinkParentItem::createNodeFromRecord(Resultset *rs)
{
    QString owner=rs->getString(1);
    QString dbLinkName=rs->getString(2);
    QString username=rs->getString(3);
    QString host=rs->getString(4);

    DbDbLinkItem *dbLink = new DbDbLinkItem(dbLinkName, dbLinkName, this->getModel(), this);
    dbLink->setSchemaName(owner);

    dbLink->setTooltip(QObject::tr("Username: %1\nHost: %2").
                    arg(username).
                    arg(host));

    return dbLink;
}

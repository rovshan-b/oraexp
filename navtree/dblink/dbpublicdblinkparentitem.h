#ifndef DBPUBLICDBLINKPARENTITEM_H
#define DBPUBLICDBLINKPARENTITEM_H

#include "dbdblinkparentitem.h"

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbPublicDbLinkParentItem : public DbDbLinkParentItem
{
public:
    DbPublicDbLinkParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual QList<DbTreeItem*> populateChildren();

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::DatabaseLinks;}
};

#endif // DBPUBLICDBLINKPARENTITEM_H

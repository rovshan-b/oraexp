#ifndef DBPUBLICDBLINKITEM_H
#define DBPUBLICDBLINKITEM_H

#include "dbdblinkitem.h"

class DbPublicDbLinkItem : public DbDbLinkItem
{
public:
    DbPublicDbLinkItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::PublicDatabaseLink;}
};

#endif // DBPUBLICDBLINKITEM_H

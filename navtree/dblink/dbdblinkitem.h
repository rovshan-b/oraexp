#ifndef DBDBLINKITEM_H
#define DBDBLINKITEM_H

#include "../dbtreeitem.h"
#include <QString>

class DbTreeModel;

class DbDbLinkItem : public DbTreeItem
{
public:
    DbDbLinkItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual QString iconName() const;

    virtual bool hasChildren() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::DatabaseLink;}

};

#endif // DBDBLINKITEM_H

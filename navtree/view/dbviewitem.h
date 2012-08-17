#ifndef DBVIEWITEM_H
#define DBVIEWITEM_H

#include "../dbtreeitem.h"
#include <QString>

class DbTreeModel;

class DbViewItem : public DbTreeItem
{
public:
    DbViewItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual bool displayChildCount() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::View;}
};

#endif // DBVIEWITEM_H

#ifndef DBTABLEINDEXPARENTITEM_H
#define DBTABLEINDEXPARENTITEM_H

#include "../index/dbindexparentitem.h"
#include <QString>

class Resultset;

class DbTableIndexParentItem : public DbIndexParentItem
{
public:
    DbTableIndexParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent);

    virtual QList<DbTreeItem*> populateChildren();

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Indexes;}
};

#endif // DBTABLEINDEXPARENTITEM_H

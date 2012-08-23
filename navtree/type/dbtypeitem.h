#ifndef DBTYPEITEM_H
#define DBTYPEITEM_H

#include "../dbtreeitem.h"
#include <QString>

class DbTreeModel;

class DbTypeItem : public DbTreeItem
{
public:
    DbTypeItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual QString iconName() const;

    virtual bool displayChildCount() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Type;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset * rs);
};

#endif // DBTYPEITEM_H

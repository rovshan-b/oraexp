#ifndef DBSYNONYMITEM_H
#define DBSYNONYMITEM_H

#include "../dbtreeitem.h"
#include <QString>

class DbTreeModel;

class DbSynonymItem : public DbTreeItem
{
public:
    DbSynonymItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual QString iconName() const;

    virtual bool hasChildren() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Synonym;}

};

#endif // DBSYNONYMITEM_H

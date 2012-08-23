#ifndef DBTYPESPECITEM_H
#define DBTYPESPECITEM_H

#include "../dbtreeitem.h"

class DbTypeSpecItem : public DbTreeItem
{
public:
    DbTypeSpecItem(const QString &m_itemText, const QString &m_itemName,
                   DbTreeModel *m_model, DbTreeItem *parent);

    QString iconName() const;
    virtual bool hasChildren() const;

    virtual bool displayChildCount() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::TypeSpec;}
};

#endif // DBTYPESPECITEM_H

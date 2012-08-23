#ifndef DBTYPEBODYITEM_H
#define DBTYPEBODYITEM_H

#include "../dbtreeitem.h"

class DbTypeBodyItem : public DbTreeItem
{
public:
    DbTypeBodyItem(const QString &m_itemText, const QString &m_itemName,
                   DbTreeModel *m_model, DbTreeItem *parent);

    QString iconName() const;
    virtual bool hasChildren() const;

    virtual bool displayChildCount() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::TypeBody;}
};

#endif // DBTYPEBODYITEM_H

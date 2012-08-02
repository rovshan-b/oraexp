#ifndef DBPACKAGESPECITEM_H
#define DBPACKAGESPECITEM_H

#include "../dbtreeitem.h"

class DbPackageSpecItem : public DbTreeItem
{
public:
    DbPackageSpecItem(const QString &m_itemText, const QString &m_itemName,
                      DbTreeModel *m_model, DbTreeItem *parent);

    QString iconName() const;
    virtual bool hasChildren() const;

    virtual bool displayChildCount() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::PackageSpec;}
};

#endif // DBPACKAGESPECITEM_H

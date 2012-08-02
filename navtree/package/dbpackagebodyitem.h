#ifndef DBPACKAGEBODYITEM_H
#define DBPACKAGEBODYITEM_H

#include "../dbtreeitem.h"

class DbPackageBodyItem : public DbTreeItem
{
public:
    DbPackageBodyItem(const QString &m_itemText, const QString &m_itemName,
                      DbTreeModel *m_model, DbTreeItem *parent);

    QString iconName() const;
    virtual bool hasChildren() const;

    virtual bool displayChildCount() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::PackageBody;}
};

#endif // DBPACKAGEBODYITEM_H

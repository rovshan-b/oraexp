#ifndef DBPACKAGEITEM_H
#define DBPACKAGEITEM_H

#include "../dbtreeitem.h"

class Resultset;

class DbPackageItem : public DbTreeItem
{
public:
    DbPackageItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    virtual bool displayChildCount() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Package;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);
};

#endif // DBPACKAGEITEM_H

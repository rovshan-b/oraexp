#ifndef DBPACKAGEPARENTITEM_H
#define DBPACKAGEPARENTITEM_H

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbPackageParentItem : public DbTreeItem
{
public:
    DbPackageParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Packages;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);
};

#endif // DBPACKAGEPARENTITEM_H

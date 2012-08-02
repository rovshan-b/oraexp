#ifndef DBTYPEPARENTITEM_H
#define DBTYPEPARENTITEM_H

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbTypeParentItem : public DbTreeItem
{
public:
    DbTypeParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Types;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);
};

#endif // DBTYPEPARENTITEM_H

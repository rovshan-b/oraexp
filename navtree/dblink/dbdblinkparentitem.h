#ifndef DBDBLINKPARENTITEM_H
#define DBDBLINKPARENTITEM_H

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbDbLinkParentItem : public DbTreeItem
{
public:
    DbDbLinkParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::DatabaseLinks;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);
};

#endif // DBDBLINKPARENTITEM_H

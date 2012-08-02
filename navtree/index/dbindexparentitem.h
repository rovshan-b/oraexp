#ifndef DBINDEXPARENTITEM_H
#define DBINDEXPARENTITEM_H

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbIndexParentItem : public DbTreeItem
{
public:
    DbIndexParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::SchemaIndexes;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);
};

#endif // DBINDEXPARENTITEM_H

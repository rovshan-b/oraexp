#ifndef DBALLSCHEMASITEM_H
#define DBALLSCHEMASITEM_H

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbAllSchemasItem : public DbTreeItem
{
public:
    DbAllSchemasItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent,
                     DbTreeModel::DbTreeNodeTypes nodeTypesToDisplay);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::AllSchemas;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);

    DbTreeModel::DbTreeNodeTypes nodeTypesToDisplay;
};

#endif // DBALLSCHEMASITEM_H

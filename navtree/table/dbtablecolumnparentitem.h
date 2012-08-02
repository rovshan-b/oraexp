#ifndef DBTABLECOLUMNPARENTITEM_H
#define DBTABLECOLUMNPARENTITEM_H

#include "../dbtreeitem.h"

class Resultset;

class DbTableColumnParentItem : public DbTreeItem
{
public:
    DbTableColumnParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Columns;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);

};

#endif // DBTABLECOLUMNPARENTITEM_H

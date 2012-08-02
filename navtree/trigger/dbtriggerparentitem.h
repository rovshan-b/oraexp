#ifndef DBTRIGGERPARENTITEM_H
#define DBTRIGGERPARENTITEM_H

#include "../dbtreeitem.h"

class DbTriggerParentItem : public DbTreeItem
{
public:

    DbTriggerParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::SchemaTriggers;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);
};

#endif // DBTRIGGERPARENTITEM_H

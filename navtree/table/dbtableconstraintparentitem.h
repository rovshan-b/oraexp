#ifndef DBTABLECONSTRAINTPARENTITEM_H
#define DBTABLECONSTRAINTPARENTITEM_H

#include "../dbtreeitem.h"

class DbTableConstraintParentItem : public DbTreeItem
{
public:
    DbTableConstraintParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Constraints;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);
};

#endif // DBTABLECONSTRAINTPARENTITEM_H

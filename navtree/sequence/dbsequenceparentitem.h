#ifndef DBSEQUENCEPARENTITEM_H
#define DBSEQUENCEPARENTITEM_H

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbSequenceParentItem : public DbTreeItem
{
public:
    DbSequenceParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Sequences;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);
};

#endif // DBSEQUENCEPARENTITEM_H

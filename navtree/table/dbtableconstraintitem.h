#ifndef DBTABLECONSTRAINTITEM_H
#define DBTABLECONSTRAINTITEM_H

#include "../dbtreeitem.h"

class Resultset;

class DbTableConstraintItem : public DbTreeItem
{
public:
    DbTableConstraintItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    void setConstraintType(const QString &constraintType);

    QList<DbTreeItem*> populateChildren();

    virtual bool displayChildCount() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Constraint;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);

private:
    QString constraintType;
};

#endif // DBTABLECONSTRAINTITEM_H

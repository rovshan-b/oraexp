#ifndef DBCONSTRAINTCOLUMNITEM_H
#define DBCONSTRAINTCOLUMNITEM_H

#include "../dbtreeitem.h"

class DbConstraintColumnItem : public DbTreeItem
{
public:
    DbConstraintColumnItem(const QString &m_itemText, const QString &m_itemName, const QString &tableName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    bool hasChildren() const;

    void setConstraintType(const QString &constraintType);

    virtual QList<DbTreeItem*> populateChildren();

    virtual bool displayChildCount() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Column;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);

private:
    QString tableName;
    QString constraintType;
};

#endif // DBCONSTRAINTCOLUMNITEM_H

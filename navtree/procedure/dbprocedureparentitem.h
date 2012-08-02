#ifndef DBPROCEDUREPARENTITEM_H
#define DBPROCEDUREPARENTITEM_H

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbProcedureParentItem : public DbTreeItem
{
public:
    DbProcedureParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Procedures;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);
};

#endif // DBPROCEDUREPARENTITEM_H

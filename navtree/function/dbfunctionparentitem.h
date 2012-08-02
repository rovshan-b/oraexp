#ifndef DBFUNCTIONPARENTITEM_H
#define DBFUNCTIONPARENTITEM_H

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbFunctionParentItem : public DbTreeItem
{
public:
    DbFunctionParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Functions;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);
};

#endif // DBFUNCTIONPARENTITEM_H

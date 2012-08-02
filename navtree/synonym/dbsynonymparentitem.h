#ifndef DBSYNONYMPARENTITEM_H
#define DBSYNONYMPARENTITEM_H

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbSynonymParentItem : public DbTreeItem
{
public:
    DbSynonymParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Synonyms;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);
};

#endif // DBSYNONYMPARENTITEM_H

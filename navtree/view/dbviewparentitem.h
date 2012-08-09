#ifndef DBVIEWPARENTITEM_H
#define DBVIEWPARENTITEM_H

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbViewParentItem : public DbTreeItem
{
public:
    DbViewParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Views;}

    void setNameFilter(const QString &nameFilter){this->nameFilter=nameFilter;}

    virtual QList<QAction *> getContextMenuItems(const QModelIndex &index) const;

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);

private:
    QString nameFilter;

};

#endif // DBVIEWPARENTITEM_H

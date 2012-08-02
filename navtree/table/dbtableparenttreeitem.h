#ifndef DBTABLEPARENTTREEITEM_H
#define DBTABLEPARENTTREEITEM_H

#include "../dbtreeitem.h"
#include "enums.h"
#include <QString>

class Resultset;

class DbTableParentTreeItem : public DbTreeItem
{
public:
    DbTableParentTreeItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent);

    QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    QList<QAction*> getContextMenuItems(const QModelIndex &index) const;

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Tables;}

    void setNameFilter(const QString &nameFilter){this->nameFilter=nameFilter;}

    static QList<DbTreeItem*> createFilteredItem(const QString &sourceObjectOwner,
                                                 const QString &tableOwner,
                                                 const QString &tableName,
                                                 DbTreeModel *m_model,
                                                 DbTreeItem *parent);

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);

private:
    QString nameFilter;
};

#endif // DBTABLEPARENTTREEITEM_H

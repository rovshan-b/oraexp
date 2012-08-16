#ifndef DBPROCEDUREITEM_H
#define DBPROCEDUREITEM_H

#include "../dbtreeitem.h"
#include <QString>

class DbTreeModel;

class DbProcedureItem : public DbTreeItem
{
public:
    DbProcedureItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual QString iconName() const;

    virtual bool hasChildren() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Procedure;}

    virtual QList<QAction *> getContextMenuItems(const QModelIndex &index) const;

};

#endif // DBPROCEDUREITEM_H

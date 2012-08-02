#ifndef DBFUNCTIONITEM_H
#define DBFUNCTIONITEM_H

#include "../dbtreeitem.h"
#include <QString>

class DbTreeModel;

class DbFunctionItem : public DbTreeItem
{
public:
    DbFunctionItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual QString iconName() const;

    virtual bool hasChildren() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Function;}

};

#endif // DBFUNCTIONITEM_H

#ifndef DBSEQUENCEITEM_H
#define DBSEQUENCEITEM_H

#include "../dbtreeitem.h"
#include <QString>

class DbTreeModel;

class DbSequenceItem : public DbTreeItem
{
public:
    DbSequenceItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual QString iconName() const;

    virtual bool hasChildren() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Sequence;}

};

#endif // DBSEQUENCEITEM_H

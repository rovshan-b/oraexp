#ifndef DBTABLETRIGGERPARENTITEM_H
#define DBTABLETRIGGERPARENTITEM_H

#include "../trigger/dbtriggerparentitem.h"
#include <QString>

class Resultset;

class DbTableTriggerParentItem : public DbTriggerParentItem
{
public:
    DbTableTriggerParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual QList<DbTreeItem*> populateChildren();

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Triggers;}
};

#endif // DBTABLETRIGGERPARENTITEM_H

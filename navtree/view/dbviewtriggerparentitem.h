#ifndef DBVIEWTRIGGERPARENTITEM_H
#define DBVIEWTRIGGERPARENTITEM_H

#include "../dbtreeitem.h"
#include <QString>

class DbViewTriggerParentItem : public DbTreeItem
{
public:
    DbViewTriggerParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Triggers;}
};

#endif // DBVIEWTRIGGERPARENTITEM_H

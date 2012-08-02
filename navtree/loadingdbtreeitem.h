#ifndef LOADINGDBTREEITEM_H
#define LOADINGDBTREEITEM_H

#include "dbtreeitem.h"

class LoadingDbTreeItem : public DbTreeItem
{
public:
    LoadingDbTreeItem(DbTreeModel *m_model, DbTreeItem *parent);

    virtual bool hasChildren() const;

    virtual CheckBehavior checkBehavior() const {return NotCheckable;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Loading;}
};

#endif // LOADINGDBTREEITEM_H

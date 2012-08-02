#include "loadingdbtreeitem.h"

LoadingDbTreeItem::LoadingDbTreeItem(DbTreeModel *model, DbTreeItem *parent) : DbTreeItem(QObject::tr("Loading..."), "", model, parent)
{
}

bool LoadingDbTreeItem::hasChildren() const
{
    return false;
}

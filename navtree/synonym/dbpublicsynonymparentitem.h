#ifndef DBPUBLICSYNONYMPARENTITEM_H
#define DBPUBLICSYNONYMPARENTITEM_H

#include "dbsynonymparentitem.h"

#include "../dbtreeitem.h"
#include <QString>

class Resultset;

class DbPublicSynonymParentItem : public DbSynonymParentItem
{
public:
    DbPublicSynonymParentItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    virtual QList<DbTreeItem*> populateChildren();

    virtual CheckBehavior checkBehavior() const {return CheckableForTogglingChildren;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::PublicSynonyms;}
};

#endif // DBPUBLICSYNONYMPARENTITEM_H

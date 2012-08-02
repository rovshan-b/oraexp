#ifndef DBSCHEMAITEM_H
#define DBSCHEMAITEM_H

#include "../dbtreeitem.h"
#include "../dbtreemodel.h"
#include <QString>

class DbSchemaItem : public DbTreeItem
{
public:
    DbSchemaItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent, bool isRootItem=false);

    virtual QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual bool displayChildCount() const;

    void setNodeTypesToDisplay(DbTreeModel::DbTreeNodeTypes nodeTypesToDisplay){this->nodeTypesToDisplay=nodeTypesToDisplay;}

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Schema;}

private:
    bool isRootItem;

    DbTreeModel::DbTreeNodeTypes nodeTypesToDisplay;

};

#endif // DBSCHEMAITEM_H

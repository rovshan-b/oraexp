#ifndef DBTABLETREEITEM_H
#define DBTABLETREEITEM_H

#include "../dbtreeitem.h"
#include "enums.h"
#include <QString>

class DbTreeModel;

class DbTableTreeItem : public DbTreeItem
{
public:
    DbTableTreeItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model,
                    DbTreeItem *parent, OraExp::TableType tableType, bool isPartitioned);

    virtual QString iconName() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual bool displayChildCount() const;

    virtual QList<QAction*> getContextMenuItems(const QModelIndex &index) const;

    virtual CheckBehavior checkBehavior() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Table;}

    virtual bool canGenerateDdlForItem() const;

    void setTableType();

private:
    OraExp::TableType tableType;
    bool isPartitioned;

};

#endif // DBTABLETREEITEM_H

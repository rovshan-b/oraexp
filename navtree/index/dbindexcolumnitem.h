#ifndef DBINDEXCOLUMNITEM_H
#define DBINDEXCOLUMNITEM_H

#include "../table/dbtablecolumnitem.h"

class DbIndexColumnItem : public DbTableColumnItem
{
public:
    DbIndexColumnItem(const QString &itemText, const QString &itemName, DbTreeModel *model,
                      const QString &dataType,
                      const QString &schemaName, const QString &tableOwner,
                      const QString &tableName, bool showChildren, DbTreeItem *parent);

    virtual bool hasChildren() const;

    virtual QList<DbTreeItem*> populateChildren();

private:
    QString tableOwner;
    QString tableName;
    bool showChildren;
};

#endif // DBINDEXCOLUMNITEM_H

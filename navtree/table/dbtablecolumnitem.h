#ifndef DBTABLECOLUMNITEM_H
#define DBTABLECOLUMNITEM_H

#include "../dbtreeitem.h"
#include <QList>

class Resulset;

class DbTableColumnItem : public DbTreeItem
{
public:
    DbTableColumnItem(const QString &itemText,
                      const QString &itemName,
                      DbTreeModel *model,
                      const QString &dataType,
                      DbTreeItem *parent);

    QString iconName() const;

    virtual bool hasChildren() const;

    virtual QList<DbTreeItem*> populateChildren();

    QString dataType() const;
    void setPrimaryKey(bool isPrimaryKey);
    void setForeignKey(bool isForeignKey);
    void setCheckConstraint(bool hasCheckConstraint);
    void setUnique(bool isUnique);

    virtual bool displayChildCount() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Column;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);

private:
    QString m_dataType;
    bool isPrimaryKey;
    bool isForeignKey;
    bool hasCheckConstraint;
    bool isUnique;
};

#endif // DBTABLECOLUMNITEM_H

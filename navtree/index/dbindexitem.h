#ifndef DBINDEXITEM_H
#define DBINDEXITEM_H

#include "../dbtreeitem.h"
#include <QList>

class Resulset;

class DbIndexItem : public DbTreeItem
{
public:
    DbIndexItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;


    void setUnique(bool unique);


    virtual QList<DbTreeItem*> populateChildren();

    virtual bool displayChildCount() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Index;}

protected:
    virtual DbTreeItem *createNodeFromRecord(Resultset *);

private:
    bool isUnique;
};

#endif // DBINDEXITEM_H

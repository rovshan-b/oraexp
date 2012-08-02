#ifndef DBTRIGGERITEM_H
#define DBTRIGGERITEM_H

#include "../dbtreeitem.h"

class DbTriggerItem : public DbTreeItem
{
public:
    DbTriggerItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);

    QString iconName() const;

    virtual bool hasChildren() const;

    virtual bool displayChildCount() const;

    virtual QList<DbTreeItem*> populateChildren();

    virtual DbTreeModel::DbTreeNodeType getItemType() const {return DbTreeModel::Trigger;}

    QString getBaseObjectType() const {return this->baseObjectType;}
    void setBaseObjectType(const QString &baseObjectType) {this->baseObjectType=baseObjectType;}

    QString getBaseObjectOwner() const {return this->baseObjectOwner;}
    void setBaseObjectOwner(const QString &baseObjectOwner) {this->baseObjectOwner=baseObjectOwner;}

    QString getBaseObjectName() const {return this->baseObjectName;}
    void setBaseObjectName(const QString &baseObjectName) {this->baseObjectName=baseObjectName;}

private:
    QString baseObjectType;
    QString baseObjectOwner;
    QString baseObjectName;
};

#endif // DBTRIGGERITEM_H

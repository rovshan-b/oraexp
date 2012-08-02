#ifndef DBTREEITEM_H
#define DBTREEITEM_H

#include <QList>
#include <QString>

#include "util/param.h"
#include "dbtreemodel.h"
#include <QModelIndex>
#include <QMenu>

class Resultset;
class QAction;

class DbTreeItem
{
public:

    enum CheckBehavior
    {
        Checkable,
        NotCheckable,
        CheckableForTogglingChildren
    };

    //DbTreeItem(const QList<QVariant> &data, DbTreeItem *parent=0);
    DbTreeItem(const QString &m_itemText, const QString &m_itemName, DbTreeModel *m_model, DbTreeItem *parent=0);
    virtual ~DbTreeItem();

    DbTreeModel *getModel() const;
    void appendChild(DbTreeItem *child);

    DbTreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column, int role) const;
    int row() const;
    DbTreeItem *parent() const;

    virtual QString iconName() const;
    virtual bool hasChildren() const;

    virtual bool areChildrenPopulated() const;
    virtual QList<DbTreeItem*> populateChildren();
    virtual void setChildrenPopulated(bool populated);

    QString tooltip() const;
    void setTooltip(const QString &m_tooltip);

    QString itemName() const;

    QString itemText() const;
    void setItemText(const QString &m_itemText);

    QString schemaName() const;
    void setSchemaName(const QString &schemaName);
    bool isInCurrentUsersSchema() const;

    QString getQuery(const QString &queryName) const;

    void setInactive(bool inactive);
    virtual bool displayChildCount() const;

    void clearChildItems(bool resetPopulatedFlag=true);
    //int childCount() const;

    virtual QList<QAction*> getContextMenuItems(const QModelIndex &index) const;
    virtual QMenu *getContextMenu(const QModelIndex &index) const;

    virtual CheckBehavior checkBehavior() const;

    void setCheckState(Qt::CheckState m_checkState);
    Qt::CheckState checkState() const;

    virtual DbTreeModel::DbTreeNodeType getItemType() const=0;

    virtual bool canGenerateDdlForItem() const;

    void setIsTopLevel(bool topLevel){this->m_isTopLevel=topLevel;}
    bool isTopLevel() const {return this->m_isTopLevel;}

protected:
    QList<DbTreeItem*> createNodeForEachRecord(const QString &select, Param *param1);
    QList<DbTreeItem*> createNodeForEachRecord(const QString &select, Param *param1, Param *param2);
    QList<DbTreeItem*> createNodeForEachRecord(const QString &select, Param *param1, Param *param2, Param *param3);
    QList<DbTreeItem*> createNodeForEachRecord(const QString &select, QList<Param*> parameters=QList<Param*>());
    virtual DbTreeItem *createNodeFromRecord(Resultset *);

private:
    QList<DbTreeItem *> m_childItems;
    DbTreeItem *m_parentItem;
    bool m_childrenPopulated;
    DbTreeModel *m_model;
    QString m_itemText;
    QString m_itemName;
    QString m_tooltip;
    QString m_dbSchemaName;
    bool m_isInactive;
    Qt::CheckState m_checkState;
    bool m_isTopLevel;
};

#endif // DBTREEITEM_H

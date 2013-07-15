#ifndef DBITEMDYNAMICACTIONCREATOR_H
#define DBITEMDYNAMICACTIONCREATOR_H

#include "contextmenuactioncreator.h"
#include "navtree/dbtreemodel.h"

class DbItemDynamicActionCreator : public ContextMenuActionCreator
{
public:
    DbItemDynamicActionCreator(const QString &schemaName,
                               const QString &objectName,
                               const QString &parentObjectName,
                               const DbTreeModel::DbTreeNodeType itemType,
                               DbUiManager *uiManager);

    virtual DynamicAction *create(const QIcon &icon, const QString &caption, QObject *parent);

private:
    QString schemaName;
    QString objectName;
    QString parentObjectName;
    DbTreeModel::DbTreeNodeType itemType;
};

#endif // DBITEMDYNAMICACTIONCREATOR_H

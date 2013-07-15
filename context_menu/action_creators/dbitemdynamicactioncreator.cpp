#include "dbitemdynamicactioncreator.h"
#include "beans/dbitemdynamicaction.h"
#include "connection_page/dbuimanager.h"

DbItemDynamicActionCreator::DbItemDynamicActionCreator(const QString &schemaName,
                                                       const QString &objectName,
                                                       const QString &parentObjectName,
                                                       const DbTreeModel::DbTreeNodeType itemType,
                                                       DbUiManager *uiManager) :
    ContextMenuActionCreator(uiManager),
    schemaName(schemaName),
    objectName(objectName),
    parentObjectName(parentObjectName),
    itemType(itemType)
{
}

DynamicAction *DbItemDynamicActionCreator::create(const QIcon &icon, const QString &caption, QObject *parent)
{
    return new DbItemDynamicAction(icon,
                                   caption, schemaName, objectName, parentObjectName,
                                   itemType, uiManager, SLOT(handleDynamicAction()),
                                   parent);
}

#include "dbitemdynamicaction.h"

DbItemDynamicAction::DbItemDynamicAction(const QIcon &icon, const QString &text, const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType, const QObject *receiver, const char *slotName, QObject *parent) :
    DbItemAction(icon, text, schemaName, objectName, itemType, receiver, slotName, parent)
{
}

DbItemDynamicAction::~DbItemDynamicAction()
{
}

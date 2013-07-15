#include "dbitemaction.h"
#include <QMenu>

DbItemAction::DbItemAction(const QIcon &icon, const QString &text,
                           const QString &schemaName,
                           const QString &objectName,
                           const DbTreeModel::DbTreeNodeType itemType,
                           const QObject *receiver,
                           const char *slotName,
                           QObject *parent) :
    DynamicAction(icon, text, receiver, slotName, parent),
    schemaName(schemaName),
    objectName(objectName),
    itemType(itemType)
{
}

DbItemAction::~DbItemAction()
{
    qDebug("DbItemAction deleted");
}

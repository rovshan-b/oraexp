#include "dbitemaction.h"

DbItemAction::DbItemAction(const QIcon &icon, const QString &text,
                           const QString &schemaName,
                           const QString &objectName,
                           const DbTreeModel::DbTreeNodeType itemType,
                           const QObject *receiver,
                           const char *slotName,
                           QObject *parent) :
    QAction(icon, text, parent),
    schemaName(schemaName),
    objectName(objectName),
    itemType(itemType)
{
    if(receiver){
        connect(this, SIGNAL(triggered()), receiver, slotName);
    }
}

DbItemAction::~DbItemAction()
{
    qDebug("DbItemAction deleted");
}

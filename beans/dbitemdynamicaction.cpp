#include "dbitemdynamicaction.h"

typedef QHash<QString, QString> QStringHash;

DbItemDynamicAction::DbItemDynamicAction(const QIcon &icon, const QString &text, const QString &schemaName, const QString &objectName, const QString &parentObjectName, const DbTreeModel::DbTreeNodeType itemType, const QObject *receiver, const char *slotName, QObject *parent) :
    DbItemAction(icon, text, schemaName, objectName, itemType, receiver, slotName, parent),
    parentObjectName(parentObjectName)
{
}

DbItemDynamicAction::~DbItemDynamicAction()
{
    qDebug("DbItemDynamicAction deleted");
}

QString DbItemDynamicAction::getParentObjectName() const
{
    return this->parentObjectName;
}

void DbItemDynamicAction::setProperties()
{
    DbItemAction::setProperties();

    properties["schemaName"] = this->schemaName;
    properties["objectName"] = this->objectName;
    properties["parentObjectName"] = this->parentObjectName;
    properties["objectType"] = QString::number((int)this->itemType);
}

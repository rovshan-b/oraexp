#include "dbitemdynamicaction.h"
#include "context_menu/handlers/contextmenuhandler.h"
#include <QDebug>

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

void DbItemDynamicAction::execute(DbUiManager *uiManager)
{
    QString handlerName = properties["attribute.handler"];
    Q_ASSERT(!handlerName.isEmpty());

    qDebug() << "handler name =" << handlerName;

    int handlerTypeId = QMetaType::type(handlerName.toStdString().c_str());
    Q_ASSERT(handlerTypeId != 0);

    qDebug() << "handlerTypeId =" << handlerTypeId;

    properties["schemaName"] = this->schemaName;
    properties["objectName"] = this->objectName;
    properties["parentObjectName"] = this->parentObjectName;
    properties["objectType"] = QString::number((int)this->itemType);

    void *handlerInstance = QMetaType::construct(handlerTypeId);

    ContextMenuHandler* contextMenuHandler = static_cast<ContextMenuHandler*>(handlerInstance);
    contextMenuHandler->setUiManager(uiManager);
    contextMenuHandler->handle(properties);

    QMetaType::destroy(handlerTypeId, handlerInstance);
}

QString DbItemDynamicAction::getParentObjectName() const
{
    return this->parentObjectName;
}

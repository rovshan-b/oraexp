#include "dbitemdynamicaction.h"
#include "context_menu/handlers/contextmenuhandler.h"
#include <QDebug>

typedef QHash<QString, QString> QStringHash;

DbItemDynamicAction::DbItemDynamicAction(const QIcon &icon, const QString &text, const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType, const QObject *receiver, const char *slotName, QObject *parent) :
    DbItemAction(icon, text, schemaName, objectName, itemType, receiver, slotName, parent)
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

    void *handlerInstance = QMetaType::construct(handlerTypeId);
    ((ContextMenuHandler*)handlerInstance)->handle(uiManager, properties);
    QMetaType::destroy(handlerTypeId, handlerInstance);
}

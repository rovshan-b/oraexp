#include "dynamicaction.h"
#include "context_menu/handlers/contextmenuhandler.h"
#include <QDebug>

DynamicAction::DynamicAction(const QIcon &icon, const QString &text, const QObject *receiver, const char *slotName, QObject *parent) :
    QAction(icon, text, parent), isDefault(false)
{
    if(receiver){
        connect(this, SIGNAL(triggered()), receiver, slotName);
    }
}

void DynamicAction::execute(DbUiManager *uiManager)
{
    QString handlerName = properties["attribute.handler"];
    Q_ASSERT(!handlerName.isEmpty());

    qDebug() << "handler name =" << handlerName;

    int handlerTypeId = QMetaType::type(handlerName.toStdString().c_str());
    Q_ASSERT(handlerTypeId != 0);

    qDebug() << "handlerTypeId =" << handlerTypeId;

    void *handlerInstance = QMetaType::construct(handlerTypeId);

    ContextMenuHandler* contextMenuHandler = static_cast<ContextMenuHandler*>(handlerInstance);
    contextMenuHandler->setUiManager(uiManager);

    setProperties();

    contextMenuHandler->handle(resultListener, properties);

    QMetaType::destroy(handlerTypeId, handlerInstance);
}

void DynamicAction::setProperties()
{

}

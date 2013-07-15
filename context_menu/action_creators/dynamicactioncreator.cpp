#include "dynamicactioncreator.h"
#include "beans/dynamicaction.h"
#include "connection_page/dbuimanager.h"

DynamicActionCreator::DynamicActionCreator(DbUiManager *uiManager, const QHash<QString, QString> &properties) :
    ContextMenuActionCreator(uiManager),
    properties(properties)
{
}

DynamicAction *DynamicActionCreator::create(const QIcon &icon, const QString &caption, QObject *parent)
{
    DynamicAction *action = new DynamicAction(icon, caption, uiManager, SLOT(handleDynamicAction()), parent);

    action->properties = this->properties;

    return action;
}

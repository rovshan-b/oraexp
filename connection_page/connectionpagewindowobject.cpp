#include "connectionpagewindowobject.h"
#include "util/widgethelper.h"
#include <QWidget>

ConnectionPageWindowObject::ConnectionPageWindowObject(DbUiManager *uiManager) :
    ConnectionPageObject(uiManager)
{
}

ConnectionPageWindowObject::~ConnectionPageWindowObject()
{
    WidgetHelper::deleteChildren(this);
}

void ConnectionPageWindowObject::showAndActivate()
{
    QWidget *widget = dynamic_cast<QWidget*>(this);
    Q_ASSERT(widget);

    widget->show();
    widget->activateWindow();
    widget->raise();
}

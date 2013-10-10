#include "connectionpagewindowobject.h"
#include <QWidget>

ConnectionPageWindowObject::ConnectionPageWindowObject(DbUiManager *uiManager) :
    ConnectionPageObject(uiManager)
{
}

void ConnectionPageWindowObject::showAndActivate()
{
    QWidget *widget = dynamic_cast<QWidget*>(this);
    Q_ASSERT(widget);

    widget->show();
    widget->activateWindow();
    widget->raise();
}

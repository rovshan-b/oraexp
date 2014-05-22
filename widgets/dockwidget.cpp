#include "dockwidget.h"

DockWidget::DockWidget(const QString &title, QWidget *parent, Qt::WindowFlags flags) :
    QDockWidget(title, parent, flags)
{
    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(stateChanged(bool)));
}

void DockWidget::stateChanged(bool active)
{
    this->active = active;
}

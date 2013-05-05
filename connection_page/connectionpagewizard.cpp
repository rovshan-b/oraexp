#include "connectionpagewizard.h"

ConnectionPageWizard::ConnectionPageWizard(QWidget *parent) :
    QWizard(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowFlags(Qt::Window | Qt::WindowSystemMenuHint
                   | Qt::WindowMinMaxButtonsHint
                   | Qt::WindowCloseButtonHint);
}

ConnectionPageWizard::~ConnectionPageWizard()
{
    qDebug("deleted ConnectionPageWizard");
}

void ConnectionPageWizard::emitInitCompletedSignal()
{
    emit initCompleted(this);
}

void ConnectionPageWizard::queryExecTaskCompleted(const QString &/*taskName*/)
{
    decreaseRefCount();
}

void ConnectionPageWizard::emitBusyStateChangedSignal()
{
    emit busyStateChanged(this, busy);
}

#include "connectionpagewindow.h"
#include <QtGui>

ConnectionPageWindow::ConnectionPageWindow(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowFlags(Qt::Window | Qt::WindowSystemMenuHint
                   | Qt::WindowMinMaxButtonsHint
                   | Qt::WindowCloseButtonHint);
}

ConnectionPageWindow::~ConnectionPageWindow()
{
    qDebug("deleted ConnectionPageWindow");
}

void ConnectionPageWindow::emitInitCompletedSignal()
{
    emit initCompleted(this);
}

void ConnectionPageWindow::queryExecTaskCompleted(const QString &/*taskName*/)
{
    decreaseRefCount();
}

void ConnectionPageWindow::emitBusyStateChangedSignal()
{
    emit busyStateChanged(this, busy);
}

void ConnectionPageWindow::reject()
{
    setResult(QDialog::Rejected);

    if(canClose()){
        QDialog::reject();
    }
}

void ConnectionPageWindow::closeEvent(QCloseEvent *e)
{
    e->setAccepted(canClose());
}

bool ConnectionPageWindow::canClose()
{
    return true;
}

#include "connectionpagewindow.h"
#include "util/widgethelper.h"
#include "util/dialoghelper.h"
#include <QtGui>

ConnectionPageWindow::ConnectionPageWindow(DbUiManager *uiManager, QWidget *parent) :
    QDialog(parent), ConnectionPageWindowObject(uiManager), inProgress(false)
{
    setAttribute(Qt::WA_DeleteOnClose);

    DialogHelper::showMaximizeMinimizeButtons(this);
}

ConnectionPageWindow::~ConnectionPageWindow()
{
    WidgetHelper::deleteChildren(this);

    qDebug("deleted ConnectionPageWindow");
}

QString ConnectionPageWindow::getDisplayName() const
{
    return windowTitle();
}

QIcon ConnectionPageWindow::getIcon() const
{
    return windowIcon();
}

void ConnectionPageWindow::emitInitCompletedSignal()
{
    emit initCompleted(this);
}

void ConnectionPageWindow::makeVisible()
{
    showAndActivate();
}

void ConnectionPageWindow::makeVisibleIfInProgress()
{
    if(inProgress){
        makeVisible();
    }
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
    if(inProgress){
        QMessageBox::information(this, tr("Window busy"),
                                 tr("Cannot close window when it is busy."));
        return false;
    }else{
        return true;
    }
}

void ConnectionPageWindow::setInProgress(bool inProgress)
{
    this->inProgress = inProgress;
}

bool ConnectionPageWindow::isInProgress() const
{
    return this->inProgress;
}

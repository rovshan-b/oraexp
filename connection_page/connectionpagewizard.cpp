#include "connectionpagewizard.h"
#include "connectionpagewizardpage.h"
#include "util/widgethelper.h"
#include <QtGui>

ConnectionPageWizard::ConnectionPageWizard(DbUiManager *uiManager, QWidget *parent) :
    QWizard(parent), ConnectionPageWindowObject(uiManager)
{
    setAttribute(Qt::WA_DeleteOnClose);

    setWindowFlags(Qt::Window | Qt::WindowSystemMenuHint
                   | Qt::WindowMinMaxButtonsHint
                   | Qt::WindowCloseButtonHint);
}

ConnectionPageWizard::~ConnectionPageWizard()
{
    //WidgetHelper::deleteChildren(this); --causes crash

    qDebug("deleted ConnectionPageWizard");
}

QString ConnectionPageWizard::getDisplayName() const
{
    return windowTitle();
}

QIcon ConnectionPageWizard::getIcon() const
{
    return windowIcon();
}

void ConnectionPageWizard::emitInitCompletedSignal()
{
    emit initCompleted(this);
}

void ConnectionPageWizard::reject()
{
    setResult(QDialog::Rejected);

    if(canClose()){
        QWizard::reject();
    }
}

void ConnectionPageWizard::closeEvent(QCloseEvent *e)
{
    e->setAccepted(canClose());
}

bool ConnectionPageWizard::canClose()
{
    ConnectionPageWizardPage *page = qobject_cast<ConnectionPageWizardPage*>(currentPage());
    Q_ASSERT(page);

    if(page->isBusy()){
        QMessageBox::information(this, tr("Wizard busy"),
                                 tr("Cannot exit wizard when it is busy."));
        return false;
    }else if(currentId() > 0 && result() != QDialog::Accepted){
        if(QMessageBox::question(this->window(), tr("Confirm exit"),
                                     tr("Exit wizard?"),
                                     QMessageBox::Ok|QMessageBox::Cancel)!=QMessageBox::Ok){
            return false;
        }
    }

    return true;
}

void ConnectionPageWizard::queryExecTaskCompleted(const QString &/*taskName*/)
{
    decreaseRefCount();
}

void ConnectionPageWizard::emitBusyStateChangedSignal()
{
    emit busyStateChanged(this, busy);
}

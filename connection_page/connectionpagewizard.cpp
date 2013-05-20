#include "connectionpagewizard.h"
#include "connectionpagewizardpage.h"
#include <QtGui>

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

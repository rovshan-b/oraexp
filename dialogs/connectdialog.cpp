#include "connectdialog.h"
#include "connection_page/connectionpageconnectwidget.h"
#include "util/dialoghelper.h"
#include "util/iconutil.h"
#include <QtGui>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    DialogHelper::showMaximizeMinimizeButtons(this);

    setWindowTitle(tr("Connect"));
    setWindowIcon(IconUtil::getIcon("connect"));


    QVBoxLayout *mainLayout = new QVBoxLayout();

    connectWidget = new ConnectionPageConnectWidget();

    mainLayout->addWidget(connectWidget);
    setLayout(mainLayout);

    connectWidget->focusReady();

    connect(connectWidget, SIGNAL(connected(DbConnection*,DbConnectionInfo*)), this, SLOT(connectionEstablished(DbConnection*,DbConnectionInfo*)));

    QSize hint = sizeHint();
    resize(750, hint.height());

    DialogHelper::centerWindow(this);
}

void ConnectDialog::reject()
{
    setResult(QDialog::Rejected);

    if(canClose()){
        emit closed();

        QDialog::reject();
    }
}

void ConnectDialog::connectionEstablished(DbConnection *db, DbConnectionInfo *connectionInfo)
{
    emit connected(db, connectionInfo);

    emit closed();

    this->accept();
}

bool ConnectDialog::canClose() const
{
    if(connectWidget->isBusy()){

        QMessageBox messageBox(QMessageBox::Critical,
                               tr("Connection in progress"),
                               tr("Cannot close dialog when connection is in progress."),
                               QMessageBox::Ok,
                               0,
                               Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
        messageBox.exec();

        return false;
    }

    return true;
}

void ConnectDialog::closeEvent(QCloseEvent *e)
{
    bool closing = canClose();

    if(closing){
        emit closed();
    }

    e->setAccepted(closing);
}

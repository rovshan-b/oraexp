#include "connectionselectorwidget.h"
#include "util/dialoghelper.h"
#include "widgets/lineeditwithautocompleter.h"
#include "connectivity/dbconnection.h"
#include "dialogs/connectdialog.h"
#include "beans/dbconnectioninfo.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

ConnectionSelectorWidget::ConnectionSelectorWidget(QWidget *parent) :
    LineEditWithButton(parent), db(0)
{
    lineEdit()->setReadOnly(true);
    browseButton()->setEnabled(false);

    connect(this, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(connectToServer()));
}

ConnectionSelectorWidget::~ConnectionSelectorWidget()
{
    cleanup();
}

void ConnectionSelectorWidget::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler = queryScheduler;

    browseButton()->setEnabled(true);
}

void ConnectionSelectorWidget::connectToServer()
{
    browseButton()->setEnabled(false);
    queryScheduler->increaseRefCount();

    ConnectDialog *dialog = new ConnectDialog();

    connect(dialog, SIGNAL(connected(DbConnection*,DbConnectionInfo*)), this, SLOT(connected(DbConnection*,DbConnectionInfo*)));
    connect(dialog, SIGNAL(closed()), this, SLOT(connectDialogClosed()));

    dialog->show();
    dialog->raise();
    dialog->activateWindow();
}

void ConnectionSelectorWidget::connected(DbConnection *db, DbConnectionInfo *connectionInfo)
{
    cleanup();

    this->db = db;
    lineEdit()->setText(connectionInfo->title);
    lineEdit()->setStyleSheet("image: url(:/images/green.png);");

    emit connectionEstablished(db);
}

void ConnectionSelectorWidget::connectDialogClosed()
{
    queryScheduler->decreaseRefCount();
    browseButton()->setEnabled(true);
}

void ConnectionSelectorWidget::cleanup()
{
    if(this->db!=0){
        delete this->db;
    }
}

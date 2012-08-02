#include "connectionselectorwidget.h"
#include "util/dialoghelper.h"
#include "widgets/lineeditwithautocompleter.h"
#include "connectivity/dbconnection.h"

ConnectionSelectorWidget::ConnectionSelectorWidget(QWidget *parent) :
    LineEditWithButton(parent), db(0)
{
    lineEdit()->setReadOnly(true);

    connect(this, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(connectToServer()));
}

ConnectionSelectorWidget::~ConnectionSelectorWidget()
{
    cleanup();
}

void ConnectionSelectorWidget::connectToServer()
{
    DbConnection *newDb=DialogHelper::getConnection(this->window());
    if(newDb!=0){

        cleanup();
        this->db=newDb;

        lineEdit()->setText(newDb->getTitle());
        emit connectionEstablished(newDb);
    }
}

void ConnectionSelectorWidget::cleanup()
{
    if(this->db!=0){
        delete this->db;
    }
}

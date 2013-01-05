#include "worksheet.h"
#include "connectivity/dbconnection.h"
#include <iostream>
#include <QtGui>

Worksheet::Worksheet(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageTab(uiManager, parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    worksheetWidget=new WorksheetWidget();
    mainLayout->addWidget(worksheetWidget);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

Worksheet::~Worksheet()
{
    qDebug("deleting worksheet");
}

void Worksheet::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    worksheetWidget->setQueryScheduler(this);

    emitInitCompletedSignal();
}

void Worksheet::setContents(const QString &contents)
{
    worksheetWidget->setContents(contents);
}

void Worksheet::focusAvailable()
{
    worksheetWidget->focusAvailable();
}

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

    connect(worksheetWidget, SIGNAL(autotraceTriggered(bool)), this, SLOT(autotraceTriggered(bool)));
}

Worksheet::~Worksheet()
{
    qDebug("deleting worksheet");
}

void Worksheet::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    worksheetWidget->setQueryScheduler(this);

    setAutotraceEnabled(shouldCheckAutotraceAction());

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

void Worksheet::setAutotraceEnabled(bool enabled)
{
    worksheetWidget->setAutotraceEnabled(enabled);
}

bool Worksheet::isAutotraceEnabled() const
{
    return worksheetWidget->isAutotraceEnabled();
}

void Worksheet::autotraceTriggered(bool checked)
{
    QList<ConnectionPageTab*> tabs = getPeerTabs();
    for(int i=0; i<tabs.size(); ++i){
        Worksheet *worksheet = static_cast<Worksheet*>(tabs.at(i));
        if(worksheet==this){
            continue;
        }
        worksheet->setAutotraceEnabled(checked);
    }
}

bool Worksheet::shouldCheckAutotraceAction() const
{
    QList<ConnectionPageTab*> tabs = getPeerTabs(1);
    if(tabs.size()==0){
        return false;
    }

    Worksheet *worksheet = qobject_cast<Worksheet*>(tabs.at(0));
    Q_ASSERT(worksheet);

    return worksheet->isAutotraceEnabled();
}

#include "dbobjectviewer.h"
#include "dbobjectviewertabs.h"
#include <QtGui>

DbObjectViewer::DbObjectViewer(const QString &schemaName,
                                 const QString &objectName,
                                 DbUiManager *uiManager,
                                 QWidget *parent) :
    ConnectionPageTab(uiManager, parent),
    schemaName(schemaName),
    objectName(objectName)
{
}

void DbObjectViewer::createUi()
{
    QVBoxLayout *layout=new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);

    infoTabs=getTabsWidget();
    infoTabs->createUi();

    layout->addWidget(infoTabs);
    setLayout(layout);
}

void DbObjectViewer::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    infoTabs->setQueryScheduler(this);

    emitInitCompletedSignal();
}

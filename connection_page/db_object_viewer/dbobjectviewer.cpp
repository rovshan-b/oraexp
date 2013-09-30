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

    viewerWidget=getViewerWidget();
    viewerWidget->createUi();

    if(properties.contains("attribute.tabIndex")){
        viewerWidget->setCurrentTab(properties["attribute.tabIndex"].toInt());
    }

    layout->addWidget(viewerWidget);
    setLayout(layout);
}

void DbObjectViewer::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    viewerWidget->setQueryScheduler(this);

    emitInitCompletedSignal();
}

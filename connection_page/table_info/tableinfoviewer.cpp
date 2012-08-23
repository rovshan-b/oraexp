#include "tableinfoviewer.h"
#include "tableinfoviewertabs.h"
#include <QtGui>

TableInfoViewer::TableInfoViewer(const QString &schemaName,
                                 const QString &tableName,
                                 DbUiManager *uiManager,
                                 QWidget *parent) :
    ConnectionPageTab(uiManager, parent), schemaName(schemaName), tableName(tableName)
{
    QVBoxLayout *layout=new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);

    infoTabs=new TableInfoViewerTabs(schemaName, tableName, this);
    layout->addWidget(infoTabs);

    setLayout(layout);
}

void TableInfoViewer::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    infoTabs->setQueryScheduler(this);

    emitInitCompletedSignal();

    //setBusy(false);
}

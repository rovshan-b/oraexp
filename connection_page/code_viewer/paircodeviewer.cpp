#include "paircodeviewer.h"
#include "codeviewertabs.h"
#include "util/dbutil.h"
#include <QtGui>

PairCodeViewer::PairCodeViewer(const QString &schemaName,
                               const QString &objectName,
                               DbTreeModel::DbTreeNodeType itemType,
                               DbUiManager *uiManager,
                               QWidget *parent) :
    ConnectionPageTab(uiManager, parent),
    schemaName(schemaName),
    objectName(objectName),
    itemType(itemType),
    initializedCount(0)
{

}

void PairCodeViewer::createUi()
{
    QVBoxLayout *layout=new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);

    tab=new QStackedWidget();
    tab->setContentsMargins(0,0,0,0);

    specViewer = new CodeViewerTabs(schemaName, objectName, DbUtil::getSpecType(itemType), uiManager);
    specViewer->setHasSpecBodySwitcher(true);
    specViewer->createUi();

    bodyViewer = new CodeViewerTabs(schemaName, objectName, DbUtil::getBodyType(itemType), uiManager);
    bodyViewer->setHasSpecBodySwitcher(true);
    bodyViewer->createUi();

    tab->addWidget(specViewer);
    tab->addWidget(bodyViewer);

    tab->setCurrentIndex(DbUtil::isSpecType(itemType) ? 0 : 1);

    layout->addWidget(tab);
    setLayout(layout);

    connect(specViewer, SIGNAL(specBodySwitchRequested()), this, SLOT(switchToBody()));
    connect(bodyViewer, SIGNAL(specBodySwitchRequested()), this, SLOT(switchToSpec()));
}

void PairCodeViewer::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    specViewer->setQueryScheduler(this);
    bodyViewer->setQueryScheduler(this);

    emitInitCompletedSignal();
}

void PairCodeViewer::switchToSpec()
{
    tab->setCurrentIndex(0);
}

void PairCodeViewer::switchToBody()
{
    tab->setCurrentIndex(1);
}

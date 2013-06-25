#include "sessionbrowser.h"
#include "widgets/groupeddataviewwidget.h"
#include "util/queryutil.h"
#include <QtGui>

SessionBrowser::SessionBrowser(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageTab(uiManager, parent)
{
}

void SessionBrowser::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    sessionViewer = new GroupedDataViewWidget();
    mainLayout->addWidget(sessionViewer);

    mainLayout->setContentsMargins(0,2,0,0);
    setLayout(mainLayout);
}

void SessionBrowser::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    sessionViewer->setSelectQuery(QueryUtil::getQuery("get_session_list", db));
    sessionViewer->setGroupByColumns(QStringList() << "USERNAME");
    sessionViewer->setQueryScheduler(this);

    sessionViewer->loadInfo();

    emitInitCompletedSignal();
}

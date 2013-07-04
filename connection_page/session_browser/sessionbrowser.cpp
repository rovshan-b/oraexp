#include "sessionbrowser.h"
#include "widgets/groupeddataviewwidget.h"
#include "util/queryutil.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/settings.h"
#include "util/strutil.h"
#include "widgets/toolbar.h"
#include "dialogs/columnselectordialog.h"
#include <QtGui>

#define GROUP_COLS_SETTING_KEY "SessionBrowserGroupColumns"

SessionBrowser::SessionBrowser(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageTab(uiManager, parent)
{
}

void SessionBrowser::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    sessionViewer = new GroupedDataViewWidget();

    createToolbar(mainLayout);

    splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(sessionViewer);

    splitter->addWidget(new QWidget());

    splitter->setStretchFactor(0, 1);

    mainLayout->addWidget(splitter);

    mainLayout->setContentsMargins(0,2,0,0);
    setLayout(mainLayout);

    connect(sessionViewer, SIGNAL(beforeLoadInfo()), this, SLOT(startProgress()));
    connect(sessionViewer, SIGNAL(afterLoadInfo()), this, SLOT(stopProgress()));
    connect(sessionViewer, SIGNAL(headerReady(QStringList)), this, SLOT(headerReady(QStringList)));
    connect(sessionViewer, SIGNAL(dataReady()), this, SLOT(dataReady()));
}

void SessionBrowser::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    sessionViewer->setSelectQuery(QueryUtil::getQuery("get_session_list", db));

    QStringList groupColumns;
    if(Settings::contains(GROUP_COLS_SETTING_KEY)){
        groupColumns = Settings::value(GROUP_COLS_SETTING_KEY).toStringList();
    }else{
        groupColumns.append("USERNAME");
    }
    sessionViewer->setGroupByColumns(groupColumns);

    sessionViewer->setQueryScheduler(this);

    sessionViewer->loadInfo();

    emitInitCompletedSignal();
}

void SessionBrowser::beforeClose()
{
    Settings::setValue(GROUP_COLS_SETTING_KEY, sessionViewer->getGroupByColumns());
}

void SessionBrowser::createToolbar(QVBoxLayout *mainLayout)
{
    toolbar = new ToolBar();

    toolbar->addAction(IconUtil::getIcon("refresh"), tr("Refresh"), sessionViewer, SLOT(refreshInfo()));
    groupAction = toolbar->addAction(IconUtil::getIcon("list_group"), tr("Groups"), this, SLOT(changeGroupingColumns()));
    groupAction->setEnabled(false);
    statusLabel = new QLabel(tr("  Loading..."));
    toolbar->addWidget(statusLabel);
    progressBarAction = WidgetHelper::addProgressBarAction(toolbar);

    WidgetHelper::addStretchToToolbar(toolbar);

    QActionGroup *splitDirectionGroup = WidgetHelper::addSplitDirectionActions(toolbar);
    connect(splitDirectionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeSplitDirection(QAction*)));

    mainLayout->addWidget(toolbar);
}

void SessionBrowser::startProgress()
{
    setInProgress(true);
}

void SessionBrowser::stopProgress()
{
    setInProgress(false);
}

void SessionBrowser::setInProgress(bool inProgress)
{
    WidgetHelper::enableToolBarActions(toolbar, !inProgress);

    if(inProgress){
        statusLabel->setText(tr("  Loading..."));
    }

    progressBarAction->setEnabled(inProgress);
    progressBarAction->setVisible(inProgress);
}

void SessionBrowser::changeGroupingColumns()
{
    ColumnSelectorDialog dialog(humanizeList(this->columnTitles),
                                humanizeList(sessionViewer->getGroupByColumns()),
                                this,
                                false);
    dialog.setWindowTitle(tr("Group by columns"));
    dialog.setAllowEmptySelection();
    if(dialog.exec()){
        sessionViewer->setGroupByColumns(unhumanizeList(dialog.getSelectedColumns()));
        sessionViewer->refreshInfo();
    }
}

void SessionBrowser::headerReady(const QStringList &columnTitles)
{
    this->columnTitles = columnTitles;
    groupAction->setEnabled(true);
}

void SessionBrowser::dataReady()
{
    statusLabel->setText(QString("  %1 session(s)").arg(sessionViewer->getRecordCount()));
}

void SessionBrowser::changeSplitDirection(QAction *action)
{
    if(!action->isChecked()){
        return;
    }

    splitter->setOrientation((Qt::Orientation)action->data().toInt());
}

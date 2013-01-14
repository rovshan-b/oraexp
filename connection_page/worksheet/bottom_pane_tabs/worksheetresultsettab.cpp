#include "worksheetresultsettab.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "connectivity/statement.h"
#include <QtGui>

WorksheetResultsetTab::WorksheetResultsetTab(QWidget *parent) :
    WorksheetBottomPaneTab(parent)
{
    QVBoxLayout *layout=new QVBoxLayout();

    QToolBar *toolbar=new QToolBar();
    toolbar->setIconSize(QSize(16, 16));
    toolbar->addAction(IconUtil::getIcon("pin"), tr("Pin"), this, SLOT(setPinned(bool)))->setCheckable(true);

    toolbar->addSeparator();

    progressBarAction=WidgetHelper::addProgressBarAction(toolbar);
    progressBarAction->setVisible(false);

    layout->addWidget(toolbar);

    resultsTable=new DataTable();
    resultsTable->setAlternatingRowColors(true);
    layout->addWidget(resultsTable);

    layout->setContentsMargins(2, 0, 2, 0);
    setLayout(layout);

    connect(resultsTable, SIGNAL(firstFetchCompleted()), this, SLOT(firstFetchCompleted()));
}

WorksheetResultPane::WorksheetBottomPaneTabType WorksheetResultsetTab::getTabType() const
{
    return WorksheetResultPane::ResultsetTab;
}

void WorksheetResultsetTab::showQueryResults(IQueryScheduler *, const QueryResult &)
{
    Q_ASSERT(false);
}

void WorksheetResultsetTab::displayResultset(IQueryScheduler *queryScheduler, Resultset *rs)
{
    progressBarAction->setVisible(true);
    resultsTable->setResultset(queryScheduler, rs);
}

void WorksheetResultsetTab::firstFetchCompleted()
{
    progressBarAction->setVisible(false);
}

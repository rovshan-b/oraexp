#include "worksheetresultsettab.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include "dialogs/dataexportdialog.h"
#include <QtGui>

WorksheetResultsetTab::WorksheetResultsetTab(QWidget *parent) :
    WorksheetBottomPaneTab(parent)
{
    QVBoxLayout *layout=new QVBoxLayout();

    createToolbar();

    layout->addWidget(toolbar);

    resultsTable=new DataTable();
    resultsTable->setAlternatingRowColors(true);
    layout->addWidget(resultsTable);

    layout->setContentsMargins(2, 0, 2, 0);
    setLayout(layout);

    connect(resultsTable, SIGNAL(firstFetchCompleted()), this, SLOT(firstFetchCompleted()));
}

void WorksheetResultsetTab::addTabSpecificToolbarButtons()
{
    toolbar->addAction(IconUtil::getIcon("export"), tr("Export"), this, SLOT(exportData()));
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
    resultsTable->setResultset(queryScheduler, rs, QHash<int,StatementDesc*>());
}

void WorksheetResultsetTab::firstFetchCompleted()
{
    progressBarAction->setVisible(false);
}

void WorksheetResultsetTab::exportData()
{
    DataExportDialog dialog(this);
    dialog.exec();
}

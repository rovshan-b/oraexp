#include "worksheetresultsettab.h"
#include "connectivity/dbconnection.h"
#include "widgets/datatable.h"
#include "util/iconutil.h"
#include "connection_page/data_exporter/dataexportdialog.h"
#include "connection_page/data_exporter/dataexporterthread.h"
#include "connection_page/data_exporter/exporters/dataexporterbase.h"
#include "models/resultsettablemodel.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

WorksheetResultsetTab::WorksheetResultsetTab(QWidget *parent) :
    WorksheetBottomPaneTab(parent),
    queryScheduler(0),
    exporterThread(0),
    exportDialog(0),
    cursorClosed(false)
{
    QVBoxLayout *layout=new QVBoxLayout();

    createToolbar();
    statusBarLabel = new QLabel();
    labelAction = toolbar->addWidget(statusBarLabel);
    stopProgressButton = toolbar->addAction(IconUtil::getIcon("stop"),
                                            tr("Stop"),
                                            this,
                                            SLOT(stopProgress()));

    layout->addWidget(toolbar);

    resultsTable=new DataTable();
    resultsTable->setAlternatingRowColors(true);
    layout->addWidget(resultsTable);

    layout->setContentsMargins(2, 0, 2, 0);
    setLayout(layout);

    connect(resultsTable, SIGNAL(firstFetchCompleted()), this, SLOT(firstFetchCompleted()));
}

WorksheetResultsetTab::~WorksheetResultsetTab()
{
}

void WorksheetResultsetTab::addTabSpecificToolbarButtons()
{
    dataExportAction = toolbar->addAction(IconUtil::getIcon("export"), tr("Export"), this, SLOT(exportData()));
    dataExportAction->setEnabled(false);
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
    cursorClosed = false;
    setInProgress(true);
    resultsTable->setResultset(queryScheduler, rs, QHash<int,StatementDesc*>());

    this->queryScheduler = queryScheduler;
}

void WorksheetResultsetTab::firstFetchCompleted()
{
    setInProgress(false);
}

void WorksheetResultsetTab::exportData()
{
    if(exportDialog==0){
        exportDialog = new DataExportDialog(this);
        exportDialog->exportWidget()->setTableName(this->queryScheduler->getDb()->getSchemaName(), "EXPORT_TABLE");
    }

    int startRow, startColumn, endRow, endColumn;
    resultsTable->getSelectedRange(&startRow, &startColumn, &endRow, &endColumn);
    exportDialog->exportWidget()->setSelectedRange(startRow, startColumn, endRow, endColumn);

    if(exportDialog->exec()){
        DataExporterBase *exporter = exportDialog->exportWidget()->createExporter();
        startExport(exporter);
    }
}

void WorksheetResultsetTab::startExport(DataExporterBase *exporter)
{
    Q_ASSERT(queryScheduler);
    Q_ASSERT(exporterThread==0);

    ResultsetTableModel *tableModel = qobject_cast<ResultsetTableModel*>(resultsTable->model());
    Q_ASSERT(tableModel);

    bool fetchToEnd=false;
    if(!tableModel->isAllDataFetched() && exporter->startRow==-1){
        QMessageBox::StandardButton result = QMessageBox::question(this->window(), tr("Not all records fetched"),
                              tr("Not all records are fetched.\n"
                                 "Press Yes to fetch and export all records.\n"
                                 "Press No to export only fetched records."),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::Yes);
        if(result == QMessageBox::Yes){
            fetchToEnd = true;
            tableModel->setAllDataFetched();
            cursorClosed = true;
            dataExportAction->setToolTip(tr("Export data (Cursor is closed)"));
        }
    }

    exporter->columnMetadata = tableModel->getColumnMetadata();
    exporterThread = new DataExporterThread(exporter,
                                            tableModel->getModelData(),
                                            tableModel->getResultset(),
                                            fetchToEnd, true, this);
    connect(exporterThread, SIGNAL(recordsExported(int)), this, SLOT(recordsExported(int)));
    connect(exporterThread, SIGNAL(exportComplete()), this, SLOT(exportComplete()));
    connect(exporterThread, SIGNAL(exportError(QString)), this, SLOT(exportError(QString)));

    this->queryScheduler->increaseRefCount();
    setInProgress(true, true, true);
    tableModel->setFetchInProgress(true);

    exporterThread->start();
}

void WorksheetResultsetTab::recordsExported(int count)
{
    statusBarLabel->setText(QString("  %1 records exported").arg(count));
}

void WorksheetResultsetTab::exportComplete()
{
    this->queryScheduler->decreaseRefCount();
    setInProgress(false);
    exporterThread=0;

    ResultsetTableModel *tableModel = qobject_cast<ResultsetTableModel*>(resultsTable->model());
    Q_ASSERT(tableModel);
    tableModel->setFetchInProgress(false);
}

void WorksheetResultsetTab::exportError(const QString &errorMessage)
{
    exportComplete();

    QMessageBox::critical(this->window(), tr("Error exporting data"),
                          tr("Following error occured while exporting data:\n%1").arg(errorMessage));
}

void WorksheetResultsetTab::stopProgress()
{
    Q_ASSERT(exporterThread);

    if(QMessageBox::question(this->window(),
                             tr("Stop export"),
                             tr("Do you want to stop exporting data?"),
                             QMessageBox::Ok | QMessageBox::Cancel)==QMessageBox::Ok){

        if(exporterThread){ //thread may have completed its job by the time user presses OK button
            exporterThread->stop();
            stopProgressButton->setVisible(false);
        }
    }
}

void WorksheetResultsetTab::setInProgress(bool inProgress, bool showsStatusMessage, bool cancellable)
{
    progressBarAction->setVisible(inProgress);
    dataExportAction->setEnabled(!inProgress && !cursorClosed);

    labelAction->setVisible(inProgress && showsStatusMessage);
    stopProgressButton->setVisible(inProgress && cancellable);
}

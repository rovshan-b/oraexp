#include "dataimporterimportpage.h"
#include "../dataimporter.h"
#include "../dataimporterthread.h"
#include "interfaces/iqueryscheduler.h"
#include "util/strutil.h"
#include <QtGui>

DataImporterImportPage::DataImporterImportPage(QWidget *parent) :
    QWizardPage(parent), queryScheduler(0), workerThread(0)
{
    setTitle(tr("Importing..."));
    setSubTitle(tr("Data import is in progress"));

    QHBoxLayout *mainLayout = new QHBoxLayout();

    QVBoxLayout *centerLayout = new QVBoxLayout();
    statusLabel = new QLabel(tr("Starting..."));
    centerLayout->addWidget(statusLabel);

    mainLayout->addLayout(centerLayout);
    mainLayout->setAlignment(centerLayout, Qt::AlignCenter);
    setLayout(mainLayout);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void DataImporterImportPage::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler = queryScheduler;
}

void DataImporterImportPage::initializePage()
{
    Q_ASSERT(queryScheduler);

    if(!workerThread){
        startWorkerThread();
    }
}

void DataImporterImportPage::setStatus(const QString &status)
{
    statusLabel->setText(status);
}

void DataImporterImportPage::importCompleted()
{
    deleteWorkerThread();

    setStatus(tr("Data import successfully completed. %1 records were imported in %2.\n"
                 "Press Next to review changes and COMMIT/ROLLBACK as necessary.\n"
                 "Press Finish to exit wizard").arg(importedCount).arg(formatMsecs(timer.elapsed(), true)));
}

void DataImporterImportPage::chunkImported(int chunkSize)
{
    importedCount += chunkSize;

    setStatus(tr("Imported %1 records").arg(importedCount));
}

void DataImporterImportPage::importError(const QString &taskName, const OciException &ex)
{
    deleteWorkerThread();

    QMessageBox::critical(this, tr("Data import error"),
                          tr("Task name: %1\nError: %2").arg(taskName, ex.getErrorMessage()));
    setStatus(tr("Completed with error"));
}

void DataImporterImportPage::startWorkerThread()
{
    Q_ASSERT(workerThread == 0);

    importedCount = 0;

    DataImporter *importerWizard = static_cast<DataImporter*>(wizard());

    workerThread = new DataImporterThread(field("schemaName").toString().trimmed().toUpper(),
                                          this->queryScheduler->getDb(),
                                          field("tableName").toString().trimmed().toUpper(),
                                          importerWizard->getColumnMappings(),
                                          field("beforeImportGroupBox").toBool() ? field("beforeImportQuery").toString().trimmed() : "",
                                          field("importQuery").toString(),
                                          field("afterImportGroupBox").toBool() ? field("afterImportQuery").toString().trimmed() : "",
                                          importerWizard->getImporter(),
                                          this);
    connect(workerThread, SIGNAL(statusChanged(QString)), this, SLOT(setStatus(QString)));
    connect(workerThread, SIGNAL(chunkImported(int)), this, SLOT(chunkImported(int)));
    connect(workerThread, SIGNAL(comparisonCompleted()), this, SLOT(importCompleted()));
    connect(workerThread, SIGNAL(compareError(QString,OciException)), this, SLOT(importError(QString,OciException)));

    timer.start();
    workerThread->start();
}

void DataImporterImportPage::deleteWorkerThread()
{
    Q_ASSERT(workerThread);

    workerThread->wait();
    delete workerThread;
    workerThread = 0;
}

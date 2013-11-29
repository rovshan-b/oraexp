#include "dataimporterimportpage.h"
#include "../dataimporter.h"
#include "../dataimporterthread.h"
#include "interfaces/iqueryscheduler.h"
#include "util/strutil.h"
#include "util/iconutil.h"
#include <QtGui>

DataImporterImportPage::DataImporterImportPage(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageWizardPage(uiManager, parent), queryScheduler(0), workerThread(0)
{
    setTitle(tr("Importing..."));
    setSubTitle(tr("Data import is in progress"));
    setFinalPage(true);

    QHBoxLayout *mainLayout = new QHBoxLayout();

    QVBoxLayout *centerLayout = new QVBoxLayout();

    statusLabel = new QLabel(tr("Starting..."));
    centerLayout->addWidget(statusLabel);

    stopButton = new QPushButton(IconUtil::getIcon("stop"), tr("Stop"));
    centerLayout->addWidget(stopButton);
    stopButton->setVisible(false);

    mainLayout->addLayout(centerLayout);
    mainLayout->setAlignment(centerLayout, Qt::AlignCenter);
    setLayout(mainLayout);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(stopButton, SIGNAL(clicked()), this, SLOT(stopButtonPressed()));
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

bool DataImporterImportPage::isBusy() const
{
    return workerThread != 0;
}

void DataImporterImportPage::setStatus(const QString &status)
{
    statusLabel->setText(status);
}

void DataImporterImportPage::importCompleted()
{
    bool stopped = workerThread->isStopped();

    deleteWorkerThread();

    if(stopped){
        setStatus(tr("Data import was cancelled and transaction was rolled back."));
        enableBackButton(true);
    }else{
        setStatus(tr("Data import successfully completed. %1 records were imported in %2.\n"
                     "Press Next to review changes and COMMIT/ROLLBACK as necessary.\n"
                     "Press Finish to exit wizard").arg(importedCount).arg(formatMsecs(timer.elapsed(), true)));
    }

    enableNextButton(true);
    enableCancelButton(true);
}

void DataImporterImportPage::chunkImported(int chunkSize)
{
    importedCount += chunkSize;

    setStatus(tr("Imported %1 records").arg(importedCount));
}

void DataImporterImportPage::importError(const QString &taskName, const OciException &ex)
{
    deleteWorkerThread();

    enableBackButton(true);
    enableNextButton(false);
    enableCancelButton(true);

    QMessageBox::critical(this, tr("Data import error"),
                          tr("Task name: %1\nError: %2").arg(taskName, ex.getErrorMessage()));
    setStatus(tr("Completed with error"));
}

void DataImporterImportPage::startWorkerThread()
{
    Q_ASSERT(workerThread == 0);

    QTimer::singleShot(0, this, SLOT(enableBackButton()));
    QTimer::singleShot(0, this, SLOT(enableNextButton()));
    QTimer::singleShot(0, this, SLOT(enableCancelButton()));

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

    stopButton->setEnabled(true);
    stopButton->setText(tr("Stop"));
    stopButton->setVisible(true);
}

void DataImporterImportPage::deleteWorkerThread()
{
    Q_ASSERT(workerThread);

    stopButton->setVisible(false);

    workerThread->wait();
    delete workerThread;
    workerThread = 0;
}

void DataImporterImportPage::enableBackButton(bool enable)
{
    wizard()->button(QWizard::BackButton)->setEnabled(enable);
}

void DataImporterImportPage::enableNextButton(bool enable)
{
    wizard()->button(QWizard::NextButton)->setEnabled(enable);
    wizard()->button(QWizard::FinishButton)->setEnabled(enable);
}

void DataImporterImportPage::enableCancelButton(bool enable)
{
    wizard()->button(QWizard::CancelButton)->setEnabled(enable);
}

void DataImporterImportPage::stopButtonPressed()
{
    if(QMessageBox::question(this->window(),
                             tr("Confirm cancellation"),
                             tr("Do you really want to stop data import?"),
                             QMessageBox::Ok|QMessageBox::Cancel)==QMessageBox::Ok){

        if(workerThread){
            stopButton->setEnabled(false);
            stopButton->setText(tr("Stopping..."));

            workerThread->stop();
        }else{
            stopButton->setVisible(false);
        }
    }
}

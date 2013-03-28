#include "dbobjectcomparer.h"
#include "widgets/subtabwidget.h"
#include "tabs/dbobjectcomparercomparetab.h"
#include "tabs/dbobjectcompareroptionstab.h"
#include "interfaces/idbobjectcomparehelper.h"
#include "util/strutil.h"
#include <QtGui>

DbObjectComparer::DbObjectComparer(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageTab(uiManager, parent), comparer(0), compareTab(0), optionsTab(0), resultsTab(0)
{

}

void DbObjectComparer::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(2,2,2,2);

    tabWidget=new SubTabWidget();
    tabWidget->setTabPosition(QTabWidget::South);
    tabWidget->setContentsMargins(0,0,0,0);
    createTabs();

    mainLayout->addWidget(tabWidget);

    createBottomLayout(mainLayout);

    setLayout(mainLayout);
}

void DbObjectComparer::createTabs()
{
    compareTab=createCompareTab(uiManager);
    connect(compareTab, SIGNAL(connectionEstablished(DbConnection*)), this, SLOT(targetConnectionEstablished(DbConnection*)));
    connect(compareTab, SIGNAL(uiCreated()), this, SIGNAL(stateChanged()));
    tabWidget->addTab(compareTab, compareTab->nestOptionsTab() ? tr("Options") :  tr("Objects"));
    if(this->getDb()){
        compareTab->setQueryScheduler(this);
    }

    optionsTab=createOptionsTab();

    if(compareTab->nestOptionsTab()){
        compareTab->addToBottomPaneTab(optionsTab, tr("Advanced options"));
    }else{
        tabWidget->addTab(optionsTab, tr("Options"));
    }

    resultsTab=createResultsTab();
    tabWidget->addTab(resultsTab, tr("Results"));

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SIGNAL(stateChanged()));
    connect(compareTab, SIGNAL(sourceSchemaChanged(QString)), resultsTab, SLOT(setCurrentSchema(QString)));
    connect(compareTab, SIGNAL(busyStateChanged(bool)), this, SLOT(busyStateChanged(bool)));
}

void DbObjectComparer::busyStateChanged(bool busy)
{
    this->setEnabled(!busy);
}

DbObjectComparerCompareTab *DbObjectComparer::createCompareTab(DbUiManager *uiManager)
{
    DbObjectComparerCompareTab *tab=doCreateCompareTab(uiManager);
    tab->createUi();
    return tab;
}

DbObjectComparerOptionsTab *DbObjectComparer::createOptionsTab()
{
    DbObjectComparerOptionsTab *tab=doCreateOptionsTab();
    tab->createUi();
    return tab;
}

DbObjectComparerResultsTab *DbObjectComparer::createResultsTab()
{
    DbObjectComparerResultsTab *tab=doCreateResultsTab();
    tab->createUi();
    return tab;
}

bool DbObjectComparer::canFind() const
{
    return dynamic_cast<ISearchable*>(tabWidget->currentWidget())!=0;
}

void DbObjectComparer::showSearchWidget()
{
    ISearchable *searchable=dynamic_cast<ISearchable*>(tabWidget->currentWidget());
    Q_ASSERT(searchable);
    searchable->showSearchWidget();
}

void DbObjectComparer::findNext()
{
    ISearchable *searchable=dynamic_cast<ISearchable*>(tabWidget->currentWidget());
    Q_ASSERT(searchable);
    searchable->findNext();
}

void DbObjectComparer::findPrevious()
{
    ISearchable *searchable=dynamic_cast<ISearchable*>(tabWidget->currentWidget());
    Q_ASSERT(searchable);
    searchable->findPrevious();
}

void DbObjectComparer::setConnection(DbConnection *db)
{
    ConnectionPageTab::setConnection(db);

    if(compareTab){
        compareTab->setQueryScheduler(this);
    }

    emitInitCompletedSignal();
}

void DbObjectComparer::createBottomLayout(QBoxLayout *layout)
{
    QVBoxLayout *statusLayout=new QVBoxLayout();
    statusLabel=new QLabel(tr("Ready"));
    statusLabel->setVisible(false);
    statusLayout->addWidget(statusLabel);

    progressBar=new QProgressBar();
    progressBar->setTextVisible(false);
    progressBar->setVisible(false);
    progressBar->setMaximumHeight(10);
    statusLayout->addWidget(progressBar);

    layout->addLayout(statusLayout);

    compareButton=new QPushButton(tr("Start"));
    compareButton->setEnabled(false);
    connect(compareButton, SIGNAL(clicked()), this, SLOT(startStopComparing()));

    layout->addWidget(compareButton);
}

void DbObjectComparer::showStatusControls(bool show)
{
    statusLabel->setVisible(show);
    progressBar->setVisible(show);
}

void DbObjectComparer::enableControls(bool enable)
{
    compareTab->setEnabled(enable);
    optionsTab->setEnabled(enable);
    //resultsTab->setEnabled(enable);

    compareButton->setText(enable ? tr("Start") : tr("Stop"));
    if(enable){
        compareButton->setEnabled(true);
    }
}

bool DbObjectComparer::isInProgress() const
{
    return progressBar->isVisible();
}

void DbObjectComparer::startStopComparing()
{
    if(!isInProgress()){
        startComparing();
    }else{
        stopComparing();
    }
}

void DbObjectComparer::startComparing()
{
    compareTab->beforeCompare();
    enableControls(false);
    showStatusControls(true);
    resultsTab->clearText();

    comparer=createCompareHelper(compareTab->getSourceSchemaName(),
                            this,
                            compareTab->getTargetSchemaName(),
                            compareTab,
                            optionsTab->getOptions(),
                            this);

    timer.start();
    increaseRefCount();

    progressBar->setRange(0, 0);

    comparer->compare();

    tabWidget->setCurrentWidget(resultsTab);
}

void DbObjectComparer::stopComparing()
{
    if(QMessageBox::question(this->window(), tr("Stop operation"),
                             tr("Do you really want to stop current operation?"),
                             QMessageBox::Ok|QMessageBox::Cancel)!=QMessageBox::Ok){
        return;
    }

    if(!isInProgress()){
        return;
    }

    compareButton->setEnabled(false);
    compareButton->setText(tr("Stopping..."));
    comparer->stop();
}

void DbObjectComparer::deleteCompareHelper()
{
    /*
    Q_ASSERT(comparer);
    delete comparer;
    comparer = 0;*/
}

void DbObjectComparer::statusChanged(const QString &newStatus)
{
    statusLabel->setText(newStatus);
}

void DbObjectComparer::comparisonResultAvailable(const QString &ddl)
{
    resultsTab->addText(ddl);
}

void DbObjectComparer::completed()
{
    deleteCompareHelper();
    progressBar->setVisible(false);

    statusChanged(tr("Comparison completed in %1").arg(formatMsecs(timer.elapsed())));

    enableControls(true);

    decreaseRefCount();
}

void DbObjectComparer::comparisonError(const QString &taskName, const OciException &exception)
{
    deleteCompareHelper();
    progressBar->setVisible(false);

    statusChanged(tr("Completed with error"));

    QMessageBox::critical(this->window(), tr("Comparison error"),
                          tr("Task name:%1\nError:%2").arg(taskName, exception.getErrorMessage()));

    enableControls(true);

    decreaseRefCount();
}

void DbObjectComparer::objectCountDetermined(int count)
{
    progressBar->setRange(0, count);
    progressBar->setValue(0);
}

void DbObjectComparer::chunkCompleted(int size)
{
    qDebug() << "chunkCompleted, size=" << size;
    progressBar->setValue(progressBar->value()+size);
}

void DbObjectComparer::targetConnectionEstablished(DbConnection *db)
{
    compareButton->setEnabled(true);
    optionsTab->targetConnectionEstablished(this->getDb(), db);
}

#include "worksheetexplainplantab.h"
#include "connectivity/statement.h"
#include "connectivity/dbconnection.h"
#include "interfaces/iqueryscheduler.h"
#include "beans/statementdesc.h"
#include "beans/explainplanrow.h"
#include "util/iconutil.h"
#include "util/strutil.h"
#include "util/widgethelper.h"
#include "util/queryutil.h"
#include "widgets/datatable.h"
#include "errors.h"
#include <QtGui>

#define TIME_COL_IX 6
#define CR_BUFFER_GETS_COL_IX 7

#define TREE_VIEW_IX 0
#define XPLAN_VIEW_IX 1
#define STATS_VIEW_IX 2

bool WorksheetExplainPlanTab::advancedOptionsVisible;
int WorksheetExplainPlanTab::explainPlanViewIndex=0;
int WorksheetExplainPlanTab::autotraceViewIndex=0;

WorksheetExplainPlanTab::WorksheetExplainPlanTab(QWidget *parent) :
    WorksheetBottomPaneTab(parent), autotraceMode(false)
{

}

WorksheetExplainPlanTab::~WorksheetExplainPlanTab()
{
    clearModel();
}

void WorksheetExplainPlanTab::createUi()
{
    QVBoxLayout *layout=new QVBoxLayout();

    createToolbar();
    layout->addWidget(toolbar);

    stackedWidget = new QStackedWidget();
    connect(stackedWidget, SIGNAL(currentChanged(int)), this, SLOT(currentViewChanged(int)));

    tree=new QTreeView();
    setupTree();
    stackedWidget->addWidget(tree);

    textViewer=new QPlainTextEdit();
    textViewer->setReadOnly(true);
    textViewer->setFont(WidgetHelper::getMonospaceFont(textViewer->font().pointSize()));
    textViewer->setWordWrapMode(QTextOption::NoWrap);
    stackedWidget->addWidget(textViewer);

    if(autotraceMode){
        sessionStatsTable = new DataTable();
        stackedWidget->addWidget(sessionStatsTable);
    }

    setCurrentView(autotraceMode ?
                       WorksheetExplainPlanTab::autotraceViewIndex
                     :
                       WorksheetExplainPlanTab::explainPlanViewIndex);
    layout->addWidget(stackedWidget);

    layout->setSpacing(2);
    layout->setContentsMargins(2,0,2,0);
    setLayout(layout);

    advancedOptionsAction->setChecked(WorksheetExplainPlanTab::advancedOptionsVisible);
    showAdvancedOptions(WorksheetExplainPlanTab::advancedOptionsVisible);
}

void WorksheetExplainPlanTab::addTabSpecificToolbarButtons()
{
    advancedOptionsAction=toolbar->addAction(IconUtil::getIcon("advanced_options"),
                                                      QObject::tr("Show/Hide advanced options"));
    advancedOptionsAction->setCheckable(true);
    connect(advancedOptionsAction, SIGNAL(toggled(bool)), this, SLOT(showAdvancedOptions(bool)));

    toolbar->addSeparator();

    QActionGroup *viewSwitcherActions = new QActionGroup(this);
    treeViewAction = viewSwitcherActions->addAction(IconUtil::getIcon("tree"), tr("Tree view"));
    treeViewAction->setCheckable(true);
    xplanViewAction = viewSwitcherActions->addAction(IconUtil::getIcon("dbms"), tr("DBMS_XPLAN output"));
    xplanViewAction->setCheckable(true);

    toolbar->addAction(treeViewAction);
    toolbar->addAction(xplanViewAction);

    if(autotraceMode){
        statsViewAction = viewSwitcherActions->addAction(IconUtil::getIcon("statistics"), tr("Session statistics"));
        statsViewAction->setCheckable(true);
        toolbar->addAction(statsViewAction);
    }

    connect(viewSwitcherActions, SIGNAL(triggered(QAction*)), this, SLOT(viewChangeRequested(QAction*)));
}

WorksheetResultPane::WorksheetBottomPaneTabType WorksheetExplainPlanTab::getTabType() const
{
    return this->autotraceMode ? WorksheetResultPane::AutotraceTab : WorksheetResultPane::ExplainPlanTab;
}

void WorksheetExplainPlanTab::setStatementId(const QString &statementId)
{
    this->statementId=statementId;
}

void WorksheetExplainPlanTab::showQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result)
{
    delete result.statement;

    clearModel();

    if(!queryScheduler->getDb()->supportsTimeColumnInExplainPlan()){
        tree->header()->setSectionHidden(TIME_COL_IX, true);
    }

    //first fetch data for visible widget
    getExplainPlanData(queryScheduler);
}

void WorksheetExplainPlanTab::setAutotraceMode(bool autotraceMode)
{
    this->autotraceMode=autotraceMode;
}

void WorksheetExplainPlanTab::explainPlanQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        progressBarAction->setVisible(false);
        QString errorMessage;
        if(autotraceMode && result.exception.getErrorCode()==ERR_TABLE_OR_VIEW_DOES_NOT_EXIST){
            errorMessage = QString(tr("Please, ask DBA for SELECT access on following views:\n"
                                   " - V$SQL_SESSION\n"
                                   " - V$SQL_PLAN\n"
                                   " - V$SQL_PLAN_STATISTICS_ALL\n"
                                   " - V$STATNAME\n"
                                   " - V$MYSTAT"));
        }else{
            errorMessage = result.exception.getErrorMessage();
        }

        QMessageBox::critical(this->window(), tr("Error retrieving explain plan"), errorMessage);
    }
}

void WorksheetExplainPlanTab::explainPlanRecordFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        progressBarAction->setVisible(false);
    }

    if(fetchResult.resultsetIx==0){
        planTableRecordFetched(fetchResult);
    }else{
        xplanRecordFetched(fetchResult);
    }
}

void WorksheetExplainPlanTab::explainPlanFetchCompleted(const QString & taskName, int resultsetIx)
{
    if(resultsetIx==0){
        planTableFetchCompleted(taskName);
    }else{
        xplanFetchCompleted(taskName);
    }
}

void WorksheetExplainPlanTab::planTableRecordFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching explain plan"), fetchResult.exception.getErrorMessage());
        return;
    }

    QStandardItemModel *model=static_cast<QStandardItemModel*>(tree->model());
    ExplainPlanRow *planInfo = ExplainPlanRow::createFromFetchResult(fetchResult);
    planData.append(planInfo);
    if(planInfo->level==-1){ //something wrong with data
        return;
    }

    QStandardItem *parentItem=0;
    if(planInfo->level==1){ //need to add top level item
        parentItem=model->invisibleRootItem();
    }else if(planInfo->level>1){
        parentItem=lastItemsForLevels.value(planInfo->level-1, 0);
    }

    if(parentItem==0){
        qDebug("could not find parent item in explain plan tree view");
        return;
    }

    QStandardItem *childItem = new QStandardItem(planInfo->operation);
    QString iconName=planInfo->operation.toLower().replace(' ', '_').replace('-', '_');
    QPixmap icon=IconUtil::getIcon(iconName);
    if(icon.isNull()){
        icon=IconUtil::getIcon("green_flag");
    }
    childItem->setIcon(icon);

    QStandardItem *optionsItem = new QStandardItem(planInfo->options);
    if(planInfo->options.contains("FULL")){
        optionsItem->setData(Qt::red, Qt::ForegroundRole);
    }else if(planInfo->options.contains("INDEX")){
        optionsItem->setData(Qt::darkGreen, Qt::ForegroundRole);
    }

    QStandardItem *cardinalityItem=new QStandardItem(planInfo->cardinality);
    cardinalityItem->setData(Qt::AlignRight, Qt::TextAlignmentRole);

    QString bytesText = planInfo->bytes;
    bool conversionOk;
    int bytes=bytesText.toInt(&conversionOk);
    if(conversionOk){
        bytesText=humanizeSize(bytes);
    }
    QStandardItem *bytesItem=new QStandardItem(bytesText);
    bytesItem->setData(Qt::AlignRight, Qt::TextAlignmentRole);

    QStandardItem *costItem=new QStandardItem(planInfo->percentCpu.isEmpty() ?
                                                  planInfo->cost
                                                :
                                                  QString("%1 (%2)").arg(planInfo->cost, planInfo->percentCpu));
    costItem->setData(Qt::AlignRight, Qt::TextAlignmentRole);

    QStandardItem *timeItem=new QStandardItem();
    timeItem->setData(Qt::AlignRight, Qt::TextAlignmentRole);
    if(!planInfo->time.isEmpty()){
        int timeInSecs = planInfo->time.toInt(&conversionOk);
        if(conversionOk){
            timeItem->setText(formatSecs(timeInSecs));
        }
    }

    QStandardItem*otherItem=new QStandardItem();
    if(!planInfo->otherTag.isEmpty() || !planInfo->other.isEmpty()){
        otherItem->setText(QString("%1 - %2").arg(planInfo->otherTag, planInfo->other));
    }

    QList<QStandardItem *> rowItems = QList<QStandardItem *>()
                                      << childItem
                                      << new QStandardItem(planInfo->objectName)
                                      << optionsItem
                                      << cardinalityItem
                                      << bytesItem
                                      << costItem
                                      << timeItem
                                      << new QStandardItem(planInfo->crBufferGets)
                                      << new QStandardItem(planInfo->partitionStart)
                                      << new QStandardItem(planInfo->partitionStop)
                                      << new QStandardItem(planInfo->accessPredicates)
                                      << new QStandardItem(planInfo->filterPredicates)
                                      << otherItem;
    //childItem->setData(qVariantFromValue((void *) planInfo));
    lastItemsForLevels[planInfo->level]=childItem;
    parentItem->appendRow(rowItems);
}

void WorksheetExplainPlanTab::planTableFetchCompleted(const QString &)
{
    lastItemsForLevels.clear();

    tree->setUpdatesEnabled(false);
    tree->expandAll();
    for(int i=0; i<tree->header()->count(); ++i){
        tree->resizeColumnToContents(i);
        tree->header()->resizeSection(i, tree->header()->sectionSize(i)+(i<=1 ? 30 : 10));
    }
    tree->setUpdatesEnabled(true);
}

void WorksheetExplainPlanTab::xplanRecordFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        textViewer->setPlainText(fetchResult.exception.getErrorMessage());
        return;
    }

    xplanOutput.append("\n").append(fetchResult.oneRow.at(0));
}

void WorksheetExplainPlanTab::xplanFetchCompleted(const QString &)
{
    textViewer->setPlainText(xplanOutput);

    progressBarAction->setVisible(false);
}

void WorksheetExplainPlanTab::showAdvancedOptions(bool show)
{
    Q_ASSERT(tree->header()->count()>0);

    int startColIx = CR_BUFFER_GETS_COL_IX + 1;
    for(int i=startColIx; i<tree->header()->count(); ++i){
        tree->setColumnHidden(i, !show);
        if(show){
            tree->resizeColumnToContents(i);
            tree->header()->resizeSection(i, tree->header()->sectionSize(i)+10);
        }
    }

    if(WorksheetExplainPlanTab::advancedOptionsVisible != show){
        WorksheetExplainPlanTab::advancedOptionsVisible = show;
    }
}

void WorksheetExplainPlanTab::viewChangeRequested(QAction *action)
{
    int tabIx;
    if(action==treeViewAction){
        tabIx=TREE_VIEW_IX;
    }else if(action==xplanViewAction){
        tabIx=XPLAN_VIEW_IX;
    }else{
        tabIx=STATS_VIEW_IX;
    }

    stackedWidget->setCurrentIndex(tabIx);

    if(autotraceMode && WorksheetExplainPlanTab::autotraceViewIndex!=tabIx){
        WorksheetExplainPlanTab::autotraceViewIndex=tabIx;
    }else if(!autotraceMode && WorksheetExplainPlanTab::explainPlanViewIndex!=tabIx){
        WorksheetExplainPlanTab::explainPlanViewIndex=tabIx;
    }
}

void WorksheetExplainPlanTab::setCurrentView(int index)
{
    stackedWidget->setCurrentIndex(index);
    switch(index){
    case TREE_VIEW_IX:
        treeViewAction->setChecked(true);
        break;
    case XPLAN_VIEW_IX:
        xplanViewAction->setChecked(true);
        break;
    default:
        statsViewAction->setChecked(true);
        break;
    }
}

void WorksheetExplainPlanTab::currentViewChanged(int index)
{
    advancedOptionsAction->setEnabled(index==TREE_VIEW_IX);
}

void WorksheetExplainPlanTab::setupTree()
{
    //tree->setHeaderHidden(true);
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setUniformRowHeights(true);
    tree->setRootIsDecorated(true);
    tree->header()->setStretchLastSection(false);
    tree->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStandardItemModel *model=new QStandardItemModel(this);
    model->setHorizontalHeaderLabels(QStringList()
                                     << tr("Operation")
                                     << tr("Object name")
                                     << tr("Options")
                                     << tr("Rows")
                                     << tr("Bytes")
                                     << tr("Cost  (%CPU)")
                                     << tr("Time")
                                     << tr("CR buffer gets")
                                     << tr("Partition start")
                                     << tr("Partition stop")
                                     << tr("Access")
                                     << tr("Filter")
                                     << tr("Other"));
    tree->setModel(model);

    if(!autotraceMode){
        tree->header()->setSectionHidden(CR_BUFFER_GETS_COL_IX, true);
    }
}

void WorksheetExplainPlanTab::clearModel()
{
    lastItemsForLevels.clear();
    xplanOutput.clear();
    QStandardItemModel *model=static_cast<QStandardItemModel*>(tree->model());
    model->setRowCount(0);
    textViewer->clear();

    qDeleteAll(planData);
    planData.clear();
}

void WorksheetExplainPlanTab::getExplainPlanData(IQueryScheduler *queryScheduler)
{
    progressBarAction->setVisible(true);
    queryScheduler->enqueueQuery("get_explain_plan_data",
                                 QList<Param*>()
                                 << new Param("statement_id", statementId)
                                 << new Param("autotrace", autotraceMode)
                                 << new Param("rs1")
                                 << new Param("rs2"),
                                 this,
                                 "get_explain_plan_data",
                                 "explainPlanQueryCompleted",
                                 "explainPlanRecordFetched",
                                 "explainPlanFetchCompleted");

    if(autotraceMode){
        sessionStatsTable->displayQueryResults(queryScheduler,
                                               QueryUtil::getQuery("get_current_session_stats", queryScheduler->getDb()),
                                               QList<Param*>());
    }
}

#include "worksheetexplainplantab.h"
#include "connectivity/statement.h"
#include "connectivity/dbconnection.h"
#include "interfaces/iqueryscheduler.h"
#include "beans/statementdesc.h"
#include "beans/explainplanrow.h"
#include "util/iconutil.h"
#include "util/strutil.h"
#include "util/widgethelper.h"
#include <QtGui>

#define TIME_COL_IX 6

bool WorksheetExplainPlanTab::advancedOptionsVisible;
int WorksheetExplainPlanTab::stackedWidgetIndex=0;

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

    tree=new QTreeView();
    setupTree();
    stackedWidget->addWidget(tree);

    textViewer=new QPlainTextEdit();
    textViewer->setReadOnly(true);
    textViewer->setFont(WidgetHelper::getMonospaceFont(textViewer->font().pointSize()));
    textViewer->setWordWrapMode(QTextOption::NoWrap);
    stackedWidget->addWidget(textViewer);

    stackedWidget->setCurrentIndex(WorksheetExplainPlanTab::stackedWidgetIndex);
    layout->addWidget(stackedWidget);

    layout->setSpacing(2);
    layout->setContentsMargins(2,0,2,0);
    setLayout(layout);

    advancedOptionsAction->setChecked(WorksheetExplainPlanTab::advancedOptionsVisible);
    showAdvancedOptions(WorksheetExplainPlanTab::advancedOptionsVisible);

    progressBarAction->setVisible(true);
}

void WorksheetExplainPlanTab::addTabSpecificToolbarButtons()
{
    advancedOptionsAction=toolbar->addAction(IconUtil::getIcon("advanced_options"),
                                                      QObject::tr("Show/Hide advanced options"));
    advancedOptionsAction->setCheckable(true);
    connect(advancedOptionsAction, SIGNAL(toggled(bool)), this, SLOT(showAdvancedOptions(bool)));

    QAction *treeTextViewSwitcherAction=toolbar->addAction(IconUtil::getIcon("dbms"),
                                                           QObject::tr("Display DBMS_XPLAN output"));
    treeTextViewSwitcherAction->setCheckable(true);
    treeTextViewSwitcherAction->setChecked(WorksheetExplainPlanTab::stackedWidgetIndex==1);
    treeTextViewSwitcherAction->setShortcut(QKeySequence("Ctrl+M"));
    connect(treeTextViewSwitcherAction, SIGNAL(toggled(bool)), this, SLOT(showDbmsXplanOutput(bool)));

}

WorksheetResultPane::WorksheetBottomPaneTabType WorksheetExplainPlanTab::getTabType() const
{
    return WorksheetResultPane::ExplainPlanTab;
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
    if(stackedWidget->currentIndex()==0){
        getExplainPlanDataForTreeView(queryScheduler);
        getExplainPlanDataForTextView(queryScheduler);
    }else{
        getExplainPlanDataForTextView(queryScheduler);
        getExplainPlanDataForTreeView(queryScheduler);
    }
}

void WorksheetExplainPlanTab::setAutotraceMode(bool autotraceMode)
{
    this->autotraceMode=autotraceMode;
}

void WorksheetExplainPlanTab::explainPlanQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        progressBarAction->setVisible(false);
        QMessageBox::critical(this->window(), tr("Error retrieving explain plan"), result.exception.getErrorMessage());
    }
}

void WorksheetExplainPlanTab::explainPlanRecordFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        progressBarAction->setVisible(false);
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

    QStandardItem *costItem=new QStandardItem(QString("%1 (%2)").arg(planInfo->cost, planInfo->percentCpu));
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
                                      << new QStandardItem(planInfo->partitionStart)
                                      << new QStandardItem(planInfo->partitionStop)
                                      << new QStandardItem(planInfo->accessPredicates)
                                      << new QStandardItem(planInfo->filterPredicates)
                                      << otherItem;
    //childItem->setData(qVariantFromValue((void *) planInfo));
    lastItemsForLevels[planInfo->level]=childItem;
    parentItem->appendRow(rowItems);
}

void WorksheetExplainPlanTab::explainPlanFetchCompleted(const QString &)
{
    progressBarAction->setVisible(false);
    lastItemsForLevels.clear();

    tree->setUpdatesEnabled(false);
    tree->expandAll();
    for(int i=0; i<tree->header()->count(); ++i){
        tree->resizeColumnToContents(i);
        tree->header()->resizeSection(i, tree->header()->sectionSize(i)+(i<=1 ? 30 : 10));
    }
    tree->setUpdatesEnabled(true);
}

void WorksheetExplainPlanTab::xplanQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        textViewer->setPlainText(result.exception.getErrorMessage());
    }
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
}

void WorksheetExplainPlanTab::showAdvancedOptions(bool show)
{
    if(tree->header()->count()<12){
        return;
    }
    for(int i=(TIME_COL_IX+1); i<tree->header()->count(); ++i){
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

void WorksheetExplainPlanTab::showDbmsXplanOutput(bool show)
{
    int ixToShow=show ? 1 : 0;

    stackedWidget->setCurrentIndex(ixToShow);

    if(WorksheetExplainPlanTab::stackedWidgetIndex!=ixToShow){
        WorksheetExplainPlanTab::stackedWidgetIndex=ixToShow;
    }
}

void WorksheetExplainPlanTab::setupTree()
{
    //tree->setHeaderHidden(true);
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
                                     << tr("Partition start")
                                     << tr("Partition stop")
                                     << tr("Access")
                                     << tr("Filter")
                                     << tr("Other"));
    tree->setModel(model);
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

void WorksheetExplainPlanTab::getExplainPlanDataForTreeView(IQueryScheduler *queryScheduler)
{
    queryScheduler->enqueueQuery("get_explain_plan_data",
                                 QList<Param*>() << new Param("statement_id", statementId) << new Param("autotrace", autotraceMode),
                                 this,
                                 "get_explain_plan_data",
                                 "explainPlanQueryCompleted",
                                 "explainPlanRecordFetched",
                                 "explainPlanFetchCompleted",
                                 true);
}

void WorksheetExplainPlanTab::getExplainPlanDataForTextView(IQueryScheduler *queryScheduler)
{
    queryScheduler->enqueueQuery("get_dbms_xplan_output",
                                 QList<Param*>() << new Param("statement_id", statementId) << new Param("autotrace", autotraceMode),
                                 this,
                                 "get_dbms_xplan_output",
                                 "xplanQueryCompleted",
                                 "xplanRecordFetched",
                                 "xplanFetchCompleted",
                                 true);
}

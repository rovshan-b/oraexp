#include "worksheetexplainplantab.h"
#include "connectivity/statement.h"
#include "interfaces/iqueryscheduler.h"
#include "beans/statementdesc.h"
#include "beans/explainplanrow.h"
#include "util/iconutil.h"
#include "util/strutil.h"
#include <QtGui>

bool WorksheetExplainPlanTab::advancedOptionsVisible;
int WorksheetExplainPlanTab::stackedWidgetIndex=0;

WorksheetExplainPlanTab::WorksheetExplainPlanTab(QWidget *parent) :
    WorksheetBottomPaneTab(parent)
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
    QFont f("Monospace", textViewer->font().pointSize());
    f.setStyleHint(QFont::Monospace);
    textViewer->setFont(f);
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

    //first fetch data for visible widget
    if(stackedWidget->currentIndex()==0){
        getExplainPlanDataForTreeView(queryScheduler);
        getExplainPlanDataForTextView(queryScheduler);
    }else{
        getExplainPlanDataForTextView(queryScheduler);
        getExplainPlanDataForTreeView(queryScheduler);
    }
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


    QStandardItem *costItem=new QStandardItem(QString("%1 (%2)").arg(planInfo->cost, planInfo->percentCpu));
    costItem->setData(Qt::AlignRight, Qt::TextAlignmentRole);

    QString bytesText = planInfo->bytes;
    bool conversionOk;
    int bytes=bytesText.toInt(&conversionOk);
    if(conversionOk){
        bytesText=humanizeSize(bytes);
    }

    QStandardItem *bytesItem=new QStandardItem(bytesText);
    bytesItem->setData(Qt::AlignRight, Qt::TextAlignmentRole);

    QStandardItem *cardinalityItem=new QStandardItem(planInfo->cardinality);
    cardinalityItem->setData(Qt::AlignRight, Qt::TextAlignmentRole);

    QList<QStandardItem *> rowItems = QList<QStandardItem *>()
                                      << childItem
                                      << new QStandardItem(planInfo->objectName)
                                      << optionsItem
                                      << costItem
                                      << bytesItem
                                      << cardinalityItem
                                      << new QStandardItem(planInfo->accessPredicates)
                                      << new QStandardItem(planInfo->filterPredicates)
                                      << new QStandardItem(planInfo->partitionStart)
                                      << new QStandardItem(planInfo->partitionStop)
                                      << new QStandardItem(planInfo->other);
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
        tree->header()->resizeSection(i, tree->header()->sectionSize(i)+(i<=2 ? 30 : 10));
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
    if(tree->header()->count()<11){
        return;
    }
    tree->setColumnHidden(6, !show); //access
    tree->setColumnHidden(7, !show); //filter
    tree->setColumnHidden(8, !show); //partition start
    tree->setColumnHidden(9, !show); //partition stop
    tree->setColumnHidden(10, !show); //other

    if(WorksheetExplainPlanTab::advancedOptionsVisible != show){
        WorksheetExplainPlanTab::advancedOptionsVisible = show;
    }

    for(int i=6; i<tree->header()->count(); ++i){
        tree->resizeColumnToContents(i);
        tree->header()->resizeSection(i, tree->header()->sectionSize(i)+10);
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
                                     << tr("Cost  (%CPU)")
                                     << tr("Bytes")
                                     << tr("Rows")
                                     << tr("Access")
                                     << tr("Filter")
                                     << tr("Partition start")
                                     << tr("Partition stop")
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
                                 QList<Param*>() << new Param("statement_id", statementId),
                                 this,
                                 "get_explain_plan_data",
                                 "explainPlanQueryCompleted",
                                 "explainPlanRecordFetched",
                                 "explainPlanFetchCompleted");
}

void WorksheetExplainPlanTab::getExplainPlanDataForTextView(IQueryScheduler *queryScheduler)
{
    queryScheduler->enqueueQuery("get_dbms_xplan_output",
                                 QList<Param*>() << new Param("statement_id", statementId),
                                 this,
                                 "get_dbms_xplan_output",
                                 "xplanQueryCompleted",
                                 "xplanRecordFetched",
                                 "xplanFetchCompleted");
}

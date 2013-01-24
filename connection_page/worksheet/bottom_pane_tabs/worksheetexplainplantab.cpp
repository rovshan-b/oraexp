#include "worksheetexplainplantab.h"
#include "connectivity/statement.h"
#include "connectivity/recordfetcherthread.h"
#include "interfaces/iqueryscheduler.h"
#include "beans/statementdesc.h"
#include "beans/explainplanrow.h"
#include "util/iconutil.h"
#include "util/strutil.h"
#include <QtGui>

bool WorksheetExplainPlanTab::advancedOptionsVisible;

WorksheetExplainPlanTab::WorksheetExplainPlanTab(QWidget *parent) :
    WorksheetBottomPaneTab(parent), fetcherThread(0), queryScheduler(0), currStatement(0)
{

}

WorksheetExplainPlanTab::~WorksheetExplainPlanTab()
{
    if(fetcherThread!=0){
        deleteFetcherThread();
    }
    clearModel();
}

void WorksheetExplainPlanTab::createUi()
{
    QVBoxLayout *layout=new QVBoxLayout();

    createToolbar();
    layout->addWidget(toolbar);

    tree=new QTreeView();
    setupTree();
    layout->addWidget(tree);

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
}

WorksheetResultPane::WorksheetBottomPaneTabType WorksheetExplainPlanTab::getTabType() const
{
    return WorksheetResultPane::ExplainPlanTab;
}

void WorksheetExplainPlanTab::showQueryResults(IQueryScheduler *queryScheduler, const QueryResult &result)
{
    Q_ASSERT(currStatement==0);
    this->currStatement=result.statement;
    this->queryScheduler=queryScheduler;

    clearModel();

    startFetcherThread();
}

void WorksheetExplainPlanTab::recordsFetched(const QList<QStringList> &records)
{
    QStandardItemModel *model=static_cast<QStandardItemModel*>(tree->model());
    for(int i=0; i<records.size(); ++i){
        const QStringList &row = records.at(i);

        ExplainPlanRow *planInfo = ExplainPlanRow::createFromStringList(row);
        planData.append(planInfo);
        if(planInfo->level==-1){ //something wrong with data
            break;
        }

        QStandardItem *parentItem=0;
        if(planInfo->level==1){ //need to add top level item
            parentItem=model->invisibleRootItem();
        }else if(planInfo->level>1){
            parentItem=lastItemsForLevels.value(planInfo->level-1, 0);
        }

        if(parentItem==0){
            qDebug("could not find parent item in explain plan tree view");
            break;
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


        QStandardItem *costItem=new QStandardItem(planInfo->cost);
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
                                          << new QStandardItem(planInfo->partitionId)
                                          << new QStandardItem(planInfo->other);
        //childItem->setData(qVariantFromValue((void *) planInfo));
        lastItemsForLevels[planInfo->level]=childItem;
        parentItem->appendRow(rowItems);
    }
}

void WorksheetExplainPlanTab::fetchComplete()
{
    deleteFetcherThread();
    progressBarAction->setVisible(false);
    lastItemsForLevels.clear();

    tree->setUpdatesEnabled(false);
    tree->expandToDepth(4);
    for(int i=0; i<tree->header()->count(); ++i){
        tree->resizeColumnToContents(i);
        tree->header()->resizeSection(i, tree->header()->sectionSize(i)+(i<=2 ? 30 : 10));
    }
    tree->setUpdatesEnabled(true);
}

void WorksheetExplainPlanTab::fetchError(const OciException &ex)
{
    deleteFetcherThread();
    progressBarAction->setVisible(false);

    QMessageBox::critical(this->window(), tr("Error fetching explain plan"), ex.getErrorMessage());
}

void WorksheetExplainPlanTab::showAdvancedOptions(bool show)
{
    if(tree->header()->count()<10){
        return;
    }
    tree->setColumnHidden(6, !show); //access
    tree->setColumnHidden(7, !show); //filter
    tree->setColumnHidden(8, !show); //partition
    tree->setColumnHidden(9, !show); //other

    if(WorksheetExplainPlanTab::advancedOptionsVisible != show){
        WorksheetExplainPlanTab::advancedOptionsVisible = show;
    }

    for(int i=6; i<tree->header()->count(); ++i){
        tree->resizeColumnToContents(i);
        tree->header()->resizeSection(i, tree->header()->sectionSize(i)+10);
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
                                     << tr("Cost")
                                     << tr("Bytes")
                                     << tr("Cardinality")
                                     << tr("Access")
                                     << tr("Filter")
                                     << tr("Partition")
                                     << tr("Other"));
    tree->setModel(model);
}

void WorksheetExplainPlanTab::startFetcherThread()
{
    Q_ASSERT(fetcherThread==0);
    Q_ASSERT(currStatement->rsCount()==1);

    queryScheduler->increaseRefCount();

    fetcherThread=new RecordFetcherThread(queryScheduler->getDb(), currStatement->rsAt(0), -1, QHash<int,StatementDesc*>(), this);
    connect(fetcherThread, SIGNAL(recordsFetched(QList<QStringList>)), this, SLOT(recordsFetched(QList<QStringList>)));
    connect(fetcherThread, SIGNAL(fetchComplete()), this, SLOT(fetchComplete()));
    connect(fetcherThread, SIGNAL(fetchError(OciException)), this, SLOT(fetchError(OciException)));
    fetcherThread->start();
}

void WorksheetExplainPlanTab::deleteFetcherThread()
{
    Q_ASSERT(fetcherThread);

    fetcherThread->wait();
    delete fetcherThread;
    delete currStatement;
    queryScheduler->decreaseRefCount();
    fetcherThread=0;
    currStatement=0;
}

void WorksheetExplainPlanTab::clearModel()
{
    lastItemsForLevels.clear();
    QStandardItemModel *model=static_cast<QStandardItemModel*>(tree->model());
    model->setRowCount(0);

    qDeleteAll(planData);
    planData.clear();
}

#include "worksheetexplainplantab.h"
#include "connectivity/statement.h"
#include "connectivity/recordfetcherthread.h"
#include "interfaces/iqueryscheduler.h"
#include "beans/statementdesc.h"
#include "beans/explainplanrow.h"
#include "delegates/explainplandelegate.h"
#include <QtGui>

WorksheetExplainPlanTab::WorksheetExplainPlanTab(QWidget *parent) :
    WorksheetBottomPaneTab(parent), fetcherThread(0), queryScheduler(0), currStatement(0)
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

    progressBarAction->setVisible(true);
}

WorksheetExplainPlanTab::~WorksheetExplainPlanTab()
{
    if(fetcherThread!=0){
        deleteFetcherThread();
    }
    clearModel();
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

        QStandardItem *childItem=new QStandardItem(planInfo->operation);
        childItem->setData(qVariantFromValue((void *) planInfo));
        lastItemsForLevels[planInfo->level]=childItem;
        parentItem->appendRow(childItem);
    }
}

void WorksheetExplainPlanTab::fetchComplete()
{
    deleteFetcherThread();
    progressBarAction->setVisible(false);
    lastItemsForLevels.clear();

    tree->setUpdatesEnabled(false);
    tree->expandAll();
    tree->setUpdatesEnabled(true);
}

void WorksheetExplainPlanTab::fetchError(const OciException &ex)
{
    deleteFetcherThread();
    progressBarAction->setVisible(false);

    QMessageBox::critical(this->window(), tr("Error fetching explain plan"), ex.getErrorMessage());
}

void WorksheetExplainPlanTab::setupTree()
{
    tree->setHeaderHidden(true);
    //tree->setUniformRowHeights(true);
    tree->setRootIsDecorated(true);
    tree->setSelectionBehavior(QAbstractItemView::SelectRows);

    QStandardItemModel *model=new QStandardItemModel(this);
    tree->setModel(model);

    ExplainPlanDelegate *delegate=new ExplainPlanDelegate(this);
    tree->setItemDelegate(delegate);
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
    model->clear();

    qDeleteAll(planData);
    planData.clear();
}

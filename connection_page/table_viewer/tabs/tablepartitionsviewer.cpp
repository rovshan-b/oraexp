#include "tablepartitionsviewer.h"
#include "widgets/datatable.h"
#include "widgets/subtabwidget.h"
#include "util/queryutil.h"
#include "util/queryexectask.h"
#include "connectivity/dbconnection.h"
#include <QtGui>

TablePartitionsViewer::TablePartitionsViewer(QWidget *parent) :
    DbObjectViewerGenericTab("get_table_partitions_for_detailed_view", parent), isPartitioned(false), complexPartitioning(false)
{

}

void TablePartitionsViewer::createMainWidget(QLayout *layout)
{
    dt=new DataTable();
    connect(dt, SIGNAL(firstFetchCompleted()), this, SLOT(partitionListFirstFetchCompleted()));
    connect(dt, SIGNAL(asyncQueryError(OciException)), this, SLOT(queryCompleted()));

    dtSubpartitions=new DataTable();

    dtSubpartitionTemplate=new DataTable();
    connect(dtSubpartitionTemplate, SIGNAL(firstFetchCompleted()), this, SLOT(queryCompleted()));
    connect(dtSubpartitionTemplate, SIGNAL(asyncQueryError(OciException)), this, SLOT(queryCompleted()));

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->addWidget(dt);

    bottomPaneTab=new SubTabWidget();
    bottomPaneTab->addTab(dtSubpartitions, tr("Subpartitions"));
    bottomPaneTab->addTab(dtSubpartitionTemplate, tr("Subpartition template"));
    splitter->addWidget(bottomPaneTab);
    bottomPaneTab->hide();

    splitter->setStretchFactor(0, 2);
    splitter->setStretchFactor(1, 1);

    layout->addWidget(splitter);

    connect(dt, SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(currentRowChanged()));
}

void TablePartitionsViewer::loadData()
{
    queryScheduler->enqueueQuery("get_table_partitioning_info",
                     QList<Param*>()
                     << new Param(":owner", schemaName)
                     << new Param(":object_name", objectName),
                     this,
                     "get_table_partitioning_info",
                     "partitioningInfoLoaded",
                     "partitionInfoFetched",
                                 "partitionInfoFetchCompleted");
}

QList<QAction *> TablePartitionsViewer::getSpecificToolbarButtons()
{
    partitioningInfoLabel=new QLabel(tr("Loading partitioning info..."));

    QList<QAction*> list;

    QAction *separator=new QAction(this);
    separator->setSeparator(true);
    list.append(separator);

    QWidgetAction *labelAction=new QWidgetAction(this);
    labelAction->setDefaultWidget(partitioningInfoLabel);
    list.append(labelAction);

    return list;
}

void TablePartitionsViewer::partitioningInfoLoaded(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error retrieving partitioning info"),
                              result.exception.getErrorMessage());

        queryCompleted();
        return;
    }
}

void TablePartitionsViewer::partitionInfoFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching partitioning info"), fetchResult.exception.getErrorMessage());
        queryCompleted();
        return;
    }

    isPartitioned=true;
    QString subpType=fetchResult.colValue("SUBPARTITIONING_TYPE");

    QString partitioningInfo;
    if(subpType=="NONE"){
        partitioningInfo=tr("P.type: %1 (%2), P.count: %3")
                                   .arg(fetchResult.oneRow.at(0))
                                   .arg(fetchResult.oneRow.at(1))
                                   .arg(fetchResult.oneRow.at(4));
    }else{
        partitioningInfo=tr("P.type: %1 (%2), Subp.type: %3 (%4), P.count: %5")
                                   .arg(fetchResult.oneRow.at(0))
                                   .arg(fetchResult.oneRow.at(1))
                                   .arg(fetchResult.oneRow.at(2))
                                   .arg(fetchResult.oneRow.at(3))
                                   .arg(fetchResult.oneRow.at(4));
        complexPartitioning=true;
    }

    QString interval = fetchResult.colValue("INTERVAL");
    if(!interval.isEmpty()){
        partitioningInfo.append(", Interval: ").append(interval);
    }

    partitioningInfoLabel->setText(partitioningInfo);
}

void TablePartitionsViewer::partitionInfoFetchCompleted(const QString &)
{
    if(complexPartitioning){
        bottomPaneTab->show();
    }

    if(isPartitioned){
        DbObjectViewerGenericTab::loadData();
    }else{
        partitioningInfoLabel->setText(tr("This table is not partitioned"));
        queryCompleted();
    }

    if(complexPartitioning){
        QString subpartitionTemplateQuery=QueryUtil::getQuery("get_table_subpartition_template_for_detailed_view");
        dtSubpartitionTemplate->displayQueryResults(queryScheduler, subpartitionTemplateQuery, getQueryParams());
    }
}


void TablePartitionsViewer::currentRowChanged()
{
    if(!complexPartitioning){
        return;
    }

    dtSubpartitions->clear();
    dtSubpartitions->horizontalHeader()->setVisible(true);
    dtSubpartitions->verticalHeader()->setVisible(true);

    QModelIndex currentIndex=dt->currentIndex();
    if(!currentIndex.isValid()){
        return;
    }

    QList<Param*> params;
    params.append(getQueryParams());
    params.append(new Param(":partition_name", dt->model()->index(currentIndex.row(), 0).data().toString()));

    dtSubpartitions->displayQueryResults(queryScheduler, QueryUtil::getQuery("get_table_subpartitions_for_detailed_view"), params);
}

void TablePartitionsViewer::clearInfo()
{
    dtSubpartitions->clear();
    dtSubpartitions->horizontalHeader()->hide();
    dtSubpartitions->verticalHeader()->hide();

    DbObjectViewerGenericTab::clearInfo();
}

void TablePartitionsViewer::partitionListFirstFetchCompleted()
{
    if(!complexPartitioning){
        queryCompleted();
    }
}

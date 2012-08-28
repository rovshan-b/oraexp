#include "tableinfoviewertabs.h"
#include "tabs/tablecolumnsviewer.h"
#include "tabs/tabledataviewer.h"
#include "tabs/tableconstraintsviewer.h"
#include "tabs/tablereferencingfkviewer.h"
#include "tabs/tablegrantsviewer.h"
#include "tabs/tabletriggersviewer.h"
#include "tabs/tabledependenciesviewer.h"
#include "tabs/tabledetailsviewer.h"
#include "tabs/tablepartitionsviewer.h"
#include "tabs/tableindexesviewer.h"
#include "tabs/tableddlviewer.h"
#include "tableinfoviewer.h"
#include "util/iconutil.h"
#include <iostream>

#include <QTabBar>

using namespace std;

TableInfoViewerTabs::TableInfoViewerTabs(const QString &schemaName,
                                 const QString &tableName,
                                 QWidget *parent) :
    SubTabWidget(parent),
    connectionAvailable(false),
    schemaName(schemaName),
    tableName(tableName),
    queryScheduler(0)
{
    //TableInfoViewer* tableInfoViewer=static_cast<TableInfoViewer*>(parent);
    //queryScheduler=tableInfoViewer;

    //Q_ASSERT(queryScheduler);

    TableColumnsViewer *columnInfo=new TableColumnsViewer(this);
    addTab(columnInfo, IconUtil::getIcon("column"), tr("Columns"));

    TableDataViewer *dataViewer=new TableDataViewer(this);
    addTab(dataViewer, IconUtil::getIcon("data"), tr("Data"));

    TableConstraintsViewer *constraintInfo = new TableConstraintsViewer(this);
    addTab(constraintInfo, IconUtil::getIcon("constraint"), tr("Constraints"));

    TableReferencingFKViewer *refFkInfo = new TableReferencingFKViewer(this);
    addTab(refFkInfo, IconUtil::getIcon("constraint_fk"), tr("Referencing FKs"));

    TableGrantsViewer *grantInfo = new TableGrantsViewer(this);
    addTab(grantInfo, IconUtil::getIcon("granted"), tr("Grants"));

    TableTriggersViewer *triggerViewer = new TableTriggersViewer(this);
    addTab(triggerViewer, IconUtil::getIcon("trigger"), tr("Triggers"));

    TableDependenciesViewer *dependencyViewer=new TableDependenciesViewer(this);
    addTab(dependencyViewer, IconUtil::getIcon("dependency"), tr("Dependencies"));

    TableDetailsViewer *detailsViewer = new TableDetailsViewer(this);
    addTab(detailsViewer, IconUtil::getIcon("details"), tr("Details"));

    TablePartitionsViewer *partitionInfo = new TablePartitionsViewer(this);
    addTab(partitionInfo, IconUtil::getIcon("partition"), tr("Partitions"));

    TableIndexesViewer *indexInfo = new TableIndexesViewer(this);
    addTab(indexInfo, IconUtil::getIcon("index"), tr("Indexes"));

    TableDdlViewer *ddlViewer = new TableDdlViewer(this);
    addTab(ddlViewer, IconUtil::getIcon("ddl"), tr("DDL"));

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(loadTabData(int)));

}

void TableInfoViewerTabs::loadTabData(int index)
{
    if(!connectionAvailable){
        return;
    }

    Q_ASSERT(this->queryScheduler);

    TableInfoViewerWidget* selectedWidget=(TableInfoViewerWidget*)widget(index);

    if(!selectedWidget->areControlsCreated()){
        selectedWidget->createChildControls();
        selectedWidget->setQueryScheduler(this->queryScheduler);
        selectedWidget->setTableName(schemaName, tableName);
        selectedWidget->loadInfo();
    }
}

void TableInfoViewerTabs::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    connectionAvailable=true;

    loadTabData(currentIndex());
}

IQueryScheduler *TableInfoViewerTabs::scheduler() const
{
    Q_ASSERT(this->queryScheduler);
    return this->queryScheduler;
}

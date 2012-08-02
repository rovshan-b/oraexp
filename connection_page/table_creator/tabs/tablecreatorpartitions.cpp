#include "tablecreatorpartitions.h"
#include "widgets/tablecreatorpartitionstab.h"
#include "../tablecreatortabs.h"
#include "tablecreatorgeneralinfo.h"
#include "connectivity/dbconnection.h"
#include <QtGui>

TableCreatorPartitions::TableCreatorPartitions(TableCreatorTabs* tableCreator,
                                               bool editMode,
                                               bool configureForIndex,
                                               QWidget *parent) :
    TableCreatorTab(tableCreator, editMode, parent), configureForIndex(configureForIndex), originalPartitioningInfo(0), originalPartitioningInfoIsOnHeap(false)
{
    QVBoxLayout *layout=new QVBoxLayout();

    tab=new SubTabWidget();
    tab->setTabPosition(QTabWidget::West);

    partitionsTab=new TableCreatorPartitionsTab(tableCreator, OraExp::PartitionPartPartition, configureForIndex, this);
    tab->addTab(partitionsTab, tr("Partitions"));

    subpartitionsTab=new TableCreatorPartitionsTab(tableCreator, OraExp::PartitionPartSubpartition, configureForIndex, this);
    subpartitionsTab->setVisible(false);
    subpartitionsTab->setPartitionNameRetriever(partitionsTab);

    subpartitionTemplateTab=new TableCreatorPartitionsTab(tableCreator, OraExp::PartitionPartSubpartitionTemplate, configureForIndex, this);
    subpartitionTemplateTab->setVisible(false);

    layout->addWidget(tab);

    setLayout(layout);

    connect(partitionsTab, SIGNAL(partitioningStrategyChanged(int)), this, SLOT(partitionTypeChanged(int)));
    connect(partitionsTab, SIGNAL(partitionTypeChanged(int)), this, SLOT(partitionTypeChanged(int)));
    connect(subpartitionsTab, SIGNAL(partitionTypeChanged(int)), this, SLOT(partitionTypeChanged(int)));
    partitionTypeChanged(0);

    if(!configureForIndex){
        connect(tableCreator, SIGNAL(currentChanged(int)), this, SLOT(tabIndexChanged(int)));
        connect(tableCreator->getGeneralInfoTab(), SIGNAL(tableTypeChanged(OraExp::TableType)), this, SLOT(tableTypeChanged(OraExp::TableType)));

        connect(partitionsTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
        connect(subpartitionsTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
        connect(subpartitionTemplateTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    }

    if(configureForIndex){
        static_cast<SubTabWidget*>(tab)->hideTabBar();
    }
}

TableCreatorPartitions::~TableCreatorPartitions()
{
    delete partitionsTab;
    delete subpartitionsTab;
    delete subpartitionTemplateTab;

    if(originalPartitioningInfoIsOnHeap){
        Q_ASSERT(originalPartitioningInfo);
        delete originalPartitioningInfo;
        qDebug("deleted originalPartitioningInfo in ~TableCreatorPartitions");
    }
}

void TableCreatorPartitions::setConnection(DbConnection *db)
{
    TableCreatorTab::setConnection(db);

    tableCreator->scheduler()->enqueueQuery("get_tablespace_list",
                                            QList<Param*>(),
                                            this,
                                            "get_tablespace_list",
                                            "tablespaceListQueryCompleted",
                                            "tablespaceRecordFetched",
                                            "tablespaceListFetchCompleted");


    partitionsTab->setConnection(db);
    subpartitionsTab->setConnection(db);
    subpartitionTemplateTab->setConnection(db);

    //if(isEditMode() && !configureForIndex){
    //    loadPartitioningInfo();
    //}
}

void TableCreatorPartitions::tablespaceListQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Failed to load tablespace list"),
                              result.exception.getErrorMessage());
    }
}

void TableCreatorPartitions::tablespaceRecordFetched(const FetchResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Failed to retrieve tablespace list"),
                              result.exception.getErrorMessage());
        return;
    }

    this->tablespaceList.append(result.oneRow.at(0));
}

void TableCreatorPartitions::tablespaceListFetchCompleted(const QString &/*taskName*/)
{
    partitionsTab->tablespaceListAvailable();
    subpartitionsTab->tablespaceListAvailable();
    subpartitionTemplateTab->tablespaceListAvailable();
}

void TableCreatorPartitions::setTableInfo(TableInfo *tableInfo)
{
    setEditMode();

    originalPartitioningInfo=&tableInfo->partitioningInfo;
    partitionsTab->loadFromPartitioningInfo(tableInfo->partitioningInfo.partitionInfo);

    if(tableInfo->partitioningInfo.partitionInfo.partitioningStrategy==1){
        subpartitionsTab->loadFromPartitioningInfo(tableInfo->partitioningInfo.subpartitionInfo);
        subpartitionTemplateTab->populateTableWithPartitionList(tableInfo->partitioningInfo.subpartitionTemplateInfo);
    }

}

void TableCreatorPartitions::setEditMode()
{
    partitionsTab->setEditMode();
    subpartitionsTab->setEditMode();
    subpartitionTemplateTab->setEditMode();
}

void TableCreatorPartitions::tabIndexChanged(int tabIx)
{
    if(tableCreator->tabText(tabIx)==tr("Partitions")){
        emit currentTabActivated();
    }
}

void TableCreatorPartitions::tableTypeChanged(OraExp::TableType tableType)
{
    int currentPartitioningStrategyIx=partitionsTab->getPartitioningStrategy();

    if(tableType==OraExp::TableTypeIndexOrganized){
        partitionsTab->setPartitioningStrategy(0, false);

        if(currentPartitioningStrategyIx!=0){
            QMessageBox::information(this->window(), tr("Information"),
                                     tr("Current partitioning strategy was changed to "
                                        "Single-Level Partitioning on Partitions tab"));
        }
    }else{
        partitionsTab->setPartitioningStrategy(currentPartitioningStrategyIx, true);
    }
}

void TableCreatorPartitions::partitionTypeChanged(int)
{
    OraExp::PartitionType partitionType=partitionsTab->getPartitionType();

    bool subpartitions=TablePartitioningInfo::canHaveSubpartitions(partitionsTab->getPartitioningStrategy(), tableCreator->getTableType(), partitionType);

    if(subpartitions)
    {
        if(tab->indexOf(subpartitionsTab)==-1){
            addSubpartitionsTab();
        }

        //int spType=subpartitionsTab->getPartitionType();
        if(TablePartitioningInfo::canHaveSubpartitionTemplate()){
            addSubpartitionTemplateTab();
        }else{
            removeSubpartitionTemplateTab();
        }
    }
    else
    {
        removeSubpartitionsTab();
        removeSubpartitionTemplateTab();
    }

    subpartitionTemplateTab->partitionTypeComboChanged((int)subpartitionsTab->getPartitionType());

    emit ddlChanged();
}

void TableCreatorPartitions::addSubpartitionsTab()
{
    if(tab->indexOf(subpartitionsTab)==-1){
        if(!subpartitionsTab->isVisible()){
            subpartitionsTab->setVisible(true);
        }
        tab->addTab(subpartitionsTab, tr("Subpartitions"));
    }
}

void TableCreatorPartitions::removeSubpartitionsTab()
{
    int subIx=tab->indexOf(subpartitionsTab);
    if(subIx!=-1){
        tab->removeTab(subIx);
    }
}

void TableCreatorPartitions::addSubpartitionTemplateTab()
{
    if(tab->indexOf(subpartitionTemplateTab)==-1){
        if(!subpartitionTemplateTab->isVisible()){
            subpartitionTemplateTab->setVisible(true);
        }
        tab->addTab(subpartitionTemplateTab, tr("Subpartition template"));
    }
}

void TableCreatorPartitions::removeSubpartitionTemplateTab()
{
    int subPIx=tab->indexOf(subpartitionTemplateTab);
    if(subPIx!=-1){
        tab->removeTab(subPIx);
    }
}

void TableCreatorPartitions::alterQuerySucceeded(const QString &taskName)
{
    Q_ASSERT(originalPartitioningInfo);
    Q_ASSERT(taskName.startsWith("table_partition") || taskName.startsWith("table_subpartition"));

    if(taskName.startsWith("table_partition")){
        partitionsTab->alterQuerySucceeded(taskName, originalPartitioningInfo->partitionInfo);
    }else if(taskName.startsWith("table_subpartition_template")){
        subpartitionTemplateTab->alterQuerySucceeded(taskName, originalPartitioningInfo->subpartitionTemplateInfo);
    }else if(taskName.startsWith("table_subpartition")){
        subpartitionsTab->alterQuerySucceeded(taskName, originalPartitioningInfo->subpartitionInfo);
    }
}

TablePartitioningInfo TableCreatorPartitions::getPartitioningInfo(bool includeIncorrectRows) const
{
    TablePartitioningInfo info;

    info.partitionInfo=partitionsTab->partitioningInfoFromWidgets(includeIncorrectRows);
    info.subpartitionInfo=subpartitionsTab->partitioningInfoFromWidgets(includeIncorrectRows);
    info.subpartitionTemplateInfo=subpartitionTemplateTab->partitioningInfoFromWidgets(includeIncorrectRows);

    info.subpartitionTemplateInfo.type=info.subpartitionInfo.type;

    bool isPartitioned=(info.partitionInfo.type!=OraExp::PartitionTypeHash && info.partitionInfo.partitions.size()>0) ||
                       (info.partitionInfo.type==OraExp::PartitionTypeHash &&
                        (
                           (info.partitionInfo.specifiedBy==1 /*individual partitions*/ && info.partitionInfo.partitions.size()>0) ||
                           (info.partitionInfo.specifiedBy==0 /*count*/ && info.partitionInfo.partitionCount>0)
                        )
                       );

    info.isEmpty=!isPartitioned;

    return info;
}

void TableCreatorPartitions::setPartitioningInfo(const TablePartitioningInfo &pInfo, const TablePartitioningInfo &originalPInfo)
{
    if(isEditMode()){
        setEditMode();
    }

    Q_ASSERT(originalPartitioningInfo==0);
    originalPartitioningInfo=new TablePartitioningInfo();
    *originalPartitioningInfo=pInfo;
    originalPartitioningInfoIsOnHeap=true;

    partitionsTab->loadFromPartitioningInfo(pInfo.partitionInfo, originalPInfo.partitionInfo.partitions.size()-1);
    subpartitionsTab->loadFromPartitioningInfo(pInfo.subpartitionInfo, originalPInfo.subpartitionInfo.partitions.size()-1);
    subpartitionTemplateTab->loadFromPartitioningInfo(pInfo.subpartitionTemplateInfo);
}

void TableCreatorPartitions::removeIncorrectRows()
{
    partitionsTab->removeIncorrectRows();
    subpartitionsTab->removeIncorrectRows();
    subpartitionTemplateTab->removeIncorrectRows();
}

bool TableCreatorPartitions::beforeAlterTable()
{
    if(partitionsTab->getPartitioningStrategy()==1){
        return subpartitionsTab->normalizeTableData(true);
    }

    return true;
}

PartitioningInfo *TableCreatorPartitions::getPartitionPartInfo(OraExp::PartitionPart partitionPart)
{
    Q_ASSERT(originalPartitioningInfo); //must be set in setTableInfo

    PartitioningInfo *result=0;

    switch(partitionPart){
    case OraExp::PartitionPartPartition:
        result=&originalPartitioningInfo->partitionInfo;
        break;
    case OraExp::PartitionPartSubpartition:
        result=&originalPartitioningInfo->subpartitionInfo;
        break;
    case OraExp::PartitionPartSubpartitionTemplate:
        result=&originalPartitioningInfo->subpartitionTemplateInfo;
        break;
    }

    Q_ASSERT(result);

    return result;
}

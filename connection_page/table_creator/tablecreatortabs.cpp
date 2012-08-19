#include "tablecreatortabs.h"
#include "tablecreator.h"
#include "util/iconutil.h"
#include "tabs/tablecreatorgeneralinfo.h"
#include "tabs/tablecreatorcolumns.h"
#include "tabs/tablecreatorconstraints.h"
#include "tabs/tablecreatorcheckconstraints.h"
#include "tabs/tablecreatorindexes.h"
#include "tabs/tablecreatorpartitions.h"
#include "tabs/tablecreatorexternalproperties.h"
#include "tabs/tablecreatorgrants.h"
#include "metadata_loaders/table/tableinfoloader.h"
#include <QtGui>

#include <iostream>
using namespace std;

TableCreatorTabs::TableCreatorTabs(const QString &schemaName,
                                   const QString &tableName,
                                   QWidget *parent) :
            SubTabWidget(parent),
            db(0),
            schemaName(schemaName),
            originalTableName(tableName),
            originalTableInfo(0)

{
    editMode=!tableName.isEmpty();

    //general info pane
    generalInfoTab=new TableCreatorGeneralInfo(schemaName, tableName, this, editMode, this);
    QScrollArea *scrollAreaForGeneralInfoTab=new QScrollArea();
    scrollAreaForGeneralInfoTab->setWidget(generalInfoTab);
    scrollAreaForGeneralInfoTab->setWidgetResizable(true);
    addTab(scrollAreaForGeneralInfoTab, IconUtil::getIcon("table"), tr("General"));

    //column list
    columnsTab=new TableCreatorColumns(this, editMode, this);
    addTab(columnsTab, IconUtil::getIcon("column"), tr("Columns"));

    //key constraints
    keyConstraintsTab=new TableCreatorConstraints(schemaName, this, editMode, this);

    //check constraints
    checkConstraintsTab=new TableCreatorCheckConstraints(this, editMode, this);

    //indexes
    indexesTab=new TableCreatorIndexes(this, editMode, this);

    //partitions
    partitionsTab=new TableCreatorPartitions(this, editMode);
    scrollAreaForPartitionsTab=new QScrollArea();
    scrollAreaForPartitionsTab->setWidget(partitionsTab);
    scrollAreaForPartitionsTab->setWidgetResizable(true);

    //external table properties
    externalTableProps=0;
    scrollAreaForExternalTableProps=0;

    //grants
    grantsTab=new TableCreatorGrants(schemaName, this, editMode, this);
    addTab(grantsTab, IconUtil::getIcon("granted"), tr("Grants"));

    showTabsBasedOnTableType(OraExp::TableTypeHeap);

    generalInfoTab->setColumnListRetriever(columnsTab);

    connect(generalInfoTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(columnsTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(keyConstraintsTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(checkConstraintsTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(indexesTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(partitionsTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(grantsTab, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));

    connect(generalInfoTab, SIGNAL(currentSchemaChanged()), this, SLOT(currentSchemaChanged()));
    connect(generalInfoTab, SIGNAL(tableTypeChanged(OraExp::TableType)), this, SLOT(tableTypeChanged(OraExp::TableType)));

    TableCreator* tableCreator=static_cast<TableCreator*>(parent);
    this->queryScheduler=tableCreator;

    if(editMode){
        connect(tableCreator, SIGNAL(alterQuerySucceeded(QString)), this, SIGNAL(alterQuerySucceeded(QString)));
    }

}

TableCreatorTabs::~TableCreatorTabs()
{   
    //deleting partitions tab first, because it calls keyConstraintsTab and causes crash
    delete scrollAreaForPartitionsTab;

    delete generalInfoTab;
    delete columnsTab;
    delete keyConstraintsTab;
    delete checkConstraintsTab;
    delete indexesTab;
    delete externalTableProps;
    delete scrollAreaForExternalTableProps;
    delete grantsTab;

    cout << "deleted tabs" << endl;

    delete originalTableInfo;

}

void TableCreatorTabs::setConnection(DbConnection *db)
{
    this->db=db;

    generalInfoTab->setConnection(db);
    columnsTab->setConnection(db);
    keyConstraintsTab->setConnection(db);
    checkConstraintsTab->setConnection(db);
    indexesTab->setConnection(db);
    partitionsTab->setConnection(db);
    grantsTab->setConnection(db);

    if(editMode){
        TableInfoLoader *metadataLoader=new TableInfoLoader(scheduler(), getSchemaName(), getTableName(), this);
        connect(metadataLoader, SIGNAL(objectInfoReady(DbObjectInfo*,MetadataLoader*)), this, SLOT(tableInfoReady(DbObjectInfo*,MetadataLoader*)));
        connect(metadataLoader, SIGNAL(loadError(QString,OciException,MetadataLoader*)), this, SLOT(loadError(QString,OciException,MetadataLoader*)));
        metadataLoader->loadObjectInfo();
    }
}

void TableCreatorTabs::tableInfoReady(DbObjectInfo *tableInfo, MetadataLoader *loader)
{
    Q_ASSERT(originalTableInfo==0);
    originalTableInfo=static_cast<TableInfo*>(tableInfo);

    generalInfoTab->setTableInfo(originalTableInfo);
    columnsTab->setTableInfo(originalTableInfo);
    keyConstraintsTab->setTableInfo(originalTableInfo);
    checkConstraintsTab->setTableInfo(originalTableInfo);
    indexesTab->setTableInfo(originalTableInfo);

    if(!originalTableInfo->partitioningInfo.isEmpty){
        insertTab(5, scrollAreaForPartitionsTab, IconUtil::getIcon("partition"), tr("Partitions"));
        partitionsTab->setTableInfo(originalTableInfo);
    }

    if(originalTableInfo->generalInfo.tableType==OraExp::TableTypeExternal){
        externalTableProps->setTableInfo(originalTableInfo);
    }

    grantsTab->setTableInfo(originalTableInfo);

    loader->deleteLater();

    emit objectInfoLoaded();
}

void TableCreatorTabs::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    //qDebug() << "loadError, taskName=" << taskName << "error message=" << ex.getErrorMessage();
    QMessageBox::critical(this, tr("Failed to load table information"),
                          tr("Task name: %1\nError: %2").arg(taskName, ex.getErrorMessage()));

    loader->deleteLater();
}

TableCreatorGeneralInfo *TableCreatorTabs::getGeneralInfoTab()
{
    return generalInfoTab;
}

TableCreatorColumns *TableCreatorTabs::getColumnsTab()
{
    return columnsTab;
}

TableCreatorConstraints *TableCreatorTabs::getConstraintsTab()
{
    return keyConstraintsTab;
}

void TableCreatorTabs::currentSchemaChanged()
{
    QString newSchemaName=getSchemaName();

    keyConstraintsTab->currentSchemaChanged(schemaName, newSchemaName);
    indexesTab->currentSchemaChanged(schemaName, newSchemaName);
    grantsTab->currentSchemaChanged(schemaName, newSchemaName);

    this->schemaName=newSchemaName;
}

/*QList<Param*> TableCreatorTabs::getBasicQueryParams() const
{
    QString tableName=getOriginalTableName();

    QList<Param*> params;
    params.append(new Param(":owner", this->schemaName));
    params.append(new Param(":table_name", tableName));

    return params;
}*/

TableInfo TableCreatorTabs::getTableInfo() const
{
    TableInfo info;

    info.generalInfo=generalInfoTab->getTableGeneralInfo();
    info.columns=columnsTab->getColumnsInfo();
    info.constraints=keyConstraintsTab->getConstraintsInfo();
    info.checkConstraints=checkConstraintsTab->getConstraintsInfo();
    info.indexes=indexesTab->getIndexesInfo();
    if(info.generalInfo.tableType==OraExp::TableTypeExternal){
        info.externalInfo=externalTableProps->getExternalInfo();
    }
    info.partitioningInfo=partitionsTab->getPartitioningInfo(false);
    info.grants=grantsTab->getGrantsInfo();

    return info;
}

QList<QueryListItem> TableCreatorTabs::generateAlterDdl()
{
    TableInfo tableInfo=getTableInfo();
    tableInfo.setOriginalInfo(originalTableInfo);

    QHash<QString,QObject*> requesters;
    requesters["general_info"]=generalInfoTab;
    requesters["columns"]=columnsTab;
    requesters["constraints"]=keyConstraintsTab;
    requesters["check_constraints"]=checkConstraintsTab;
    requesters["indexes"]=indexesTab;
    requesters["partitions"]=partitionsTab;
    requesters["external_properties"]=externalTableProps;
    requesters["grants"]=grantsTab;

    return tableInfo.generateAlterDdl(requesters);
}

void TableCreatorTabs::removeIncorrectRows()
{
    columnsTab->removeIncorrectRows();
    keyConstraintsTab->removeIncorrectRows();
    checkConstraintsTab->removeIncorrectRows();
    indexesTab->removeIncorrectRows();

    int partitionsTabIx=indexOf(scrollAreaForPartitionsTab);
    if(partitionsTabIx!=-1){
        partitionsTab->removeIncorrectRows();
    }

    grantsTab->removeIncorrectRows();
}

bool TableCreatorTabs::beforeAlterTable()
{
    int partitionsTabIx=indexOf(scrollAreaForPartitionsTab);
    if(partitionsTabIx!=-1){
        return partitionsTab->beforeAlterTable();
    }

    return true;
}

QString TableCreatorTabs::getSchemaName() const
{
    return generalInfoTab->getSchema();
}

QString TableCreatorTabs::getTableName() const
{
    return generalInfoTab->getTableName();
}

QString TableCreatorTabs::getOriginalTableName() const
{
    return this->originalTableName;
}

void TableCreatorTabs::setOriginalTableName(const QString &tableName)
{
    this->originalTableName=tableName;
}

OraExp::TableType TableCreatorTabs::getTableType() const
{
    return generalInfoTab->getTableType();
}

void TableCreatorTabs::tableTypeChanged(OraExp::TableType newTableType)
{
    columnsTab->setTableType(newTableType);
    keyConstraintsTab->setTableType(newTableType);

    showTabsBasedOnTableType(newTableType);

    emit ddlChanged();
}

void TableCreatorTabs::showTabsBasedOnTableType(OraExp::TableType tableType)
{
    setUpdatesEnabled(false);

    if(tableType==OraExp::TableTypeExternal){
        if(scrollAreaForExternalTableProps==0){
            externalTableProps=new TableCreatorExternalProperties(db, this->getColumnsTab(), this, this->editMode, this);
            scrollAreaForExternalTableProps=new QScrollArea();
            scrollAreaForExternalTableProps->setWidget(externalTableProps);
            scrollAreaForExternalTableProps->setWidgetResizable(true);
            connect(externalTableProps, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
        }

        int lastTabIxToRemove=indexOf(grantsTab)-1;
        for(int i=lastTabIxToRemove; i>=2; --i){
            removeTab(i);
        }

        insertTab(indexOf(grantsTab), scrollAreaForExternalTableProps, IconUtil::getIcon("external_table_props"), tr("External"));

    }else{

        int externalPropsTabIx=indexOf(scrollAreaForExternalTableProps);
        if(externalPropsTabIx!=-1){
            removeTab(externalPropsTabIx); //remove external properties tab
        }

        int keyConstraintsTabIx=indexOf(keyConstraintsTab);

        if(keyConstraintsTabIx==-1){
            //and add all others
            insertTab(2, keyConstraintsTab, IconUtil::getIcon("column_pk"), tr("Keys"));
            insertTab(3, checkConstraintsTab, IconUtil::getIcon("constraint"), tr("Checks"));
            insertTab(4, indexesTab, IconUtil::getIcon("index"), tr("Indexes"));
        }

        int partitionsTabIx=indexOf(scrollAreaForPartitionsTab);
        if(partitionsTabIx==-1 &&
                tableType!=OraExp::TableTypeTemporarySession &&
                tableType!=OraExp::TableTypeTemporaryTransaction &&
                !editMode /*in edit mode partitions tab will be shown if table is partitioned in tableInfoReady slot*/){
            insertTab(5, scrollAreaForPartitionsTab, IconUtil::getIcon("partition"), tr("Partitions"));
        }else if(partitionsTabIx!=-1 &&
                 tableType!=OraExp::TableTypeHeap &&
                 tableType!=OraExp::TableTypeIndexOrganized){
            removeTab(partitionsTabIx);
        }
    }

    setUpdatesEnabled(true);
}

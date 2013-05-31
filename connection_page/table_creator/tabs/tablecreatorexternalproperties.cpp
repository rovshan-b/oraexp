#include "tablecreatorexternalproperties.h"
#include "widgets/tablecreatorexternalpropertiesgeneralinfowidget.h"
#include "widgets/oracleloaderdriverrecordpropertieswidget.h"
#include "widgets/oracleloaderdriverfieldpropertieswidget.h"
#include "widgets/oracledatapumpdriverproperties.h"
#include "widgets/tablecreatorexternalaccessparameterseditorwidget.h"
#include "connectivity/dbconnection.h"
#include "../tablecreatortabs.h"
#include "beans/tableexternalinfo.h"
#include "interfaces/iqueryscheduler.h"
#include <QtGui>

#include <iostream>
using namespace std;

TableCreatorExternalProperties::TableCreatorExternalProperties(IQueryScheduler *queryScheduler,
                                                               IStringListRetriever *columnListRetriever,
                                                               TableCreatorTabs *tableCreator,
                                                               DbObjectCreator::CreatorMode creatorMode,
                                                               QWidget *parent) :
    TableCreatorTab(tableCreator, creatorMode, parent), accessParamsEditor(0)
{
    TableCreatorTab::setQueryScheduler(queryScheduler);

    QVBoxLayout *layout=new QVBoxLayout();

    tab=new QTabWidget();
    tab->setTabPosition(QTabWidget::West);
    //tab->setDocumentMode(true);

    generalInfo=new TableCreatorExternalPropertiesGeneralInfoWidget(queryScheduler, creatorMode, tableCreator);

    if(getCreatorMode() == DbObjectCreator::EditExisting){
        accessParamsEditor=new TableCreatorExternalAccessParametersEditorWidget();
    }else{
        recordProperties=new OracleLoaderDriverRecordPropertiesWidget();
        fieldProperties=new OracleLoaderDriverFieldPropertiesWidget(queryScheduler, columnListRetriever);
        dataPumpProperties=new OracleDataPumpDriverProperties();
    }

    tab->addTab(generalInfo, tr("General"));
    //tab->addTab(recordProperties, tr("Record format"));
    //tab->addTab(fieldProperties, tr("Field definitions"));
    //tab->addTab(dataPumpProperties, tr("Data pump properties"));

    layout->addWidget(tab);

    setLayout(layout);

    connect(generalInfo, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));

    if(getCreatorMode() == DbObjectCreator::EditExisting){
        tab->addTab(accessParamsEditor, tr("Access parameters"));

        connect(accessParamsEditor, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));

        //QTimer::singleShot(0, this, SLOT(loadExternalTableInfo()));

    }else{
        driverChanged(0);

        connect(generalInfo, SIGNAL(driverChanged(int)), this, SLOT(driverChanged(int)));
        connect(generalInfo, SIGNAL(parallelChanged(ComboBoxAndLineEditWidget*)), recordProperties, SLOT(parallelEditorChanged(ComboBoxAndLineEditWidget*)));
        connect(recordProperties, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
        connect(fieldProperties, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
        connect(dataPumpProperties, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    }

    populateDirectoryList();
}

TableCreatorExternalProperties::~TableCreatorExternalProperties()
{
    delete generalInfo;

    if(accessParamsEditor){
        delete accessParamsEditor;
    }else{
        delete recordProperties;
        delete fieldProperties;
        delete dataPumpProperties;
    }
    cout << "deleted external table child tabs" << endl;
}

void TableCreatorExternalProperties::driverChanged(int selectedIndex)
{
    if(selectedIndex==0){
        if(tab->count()>1){
            tab->removeTab(1);
        }
        tab->addTab(recordProperties, tr("Record format"));
        tab->addTab(fieldProperties, tr("Field definitions"));
    }else{
        if(tab->count()>1){
            tab->removeTab(2);
            tab->removeTab(1);
        }
        tab->addTab(dataPumpProperties, tr("Data pump properties"));
    }
}

void TableCreatorExternalProperties::populateDirectoryList()
{
    this->queryScheduler->enqueueQuery("get_directory_list",
                                            QList<Param*>(),
                                            this,
                                            "get_directory_list",
                                            "dirListQueryCompleted",
                                            "dirRecordFetched",
                                            "dirListFetchCompleted");
}

void TableCreatorExternalProperties::dirListQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Failed to load directory list"),
                              result.exception.getErrorMessage());
    }
}

void TableCreatorExternalProperties::dirRecordFetched(const FetchResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Failed to retrieve directory list"),
                              result.exception.getErrorMessage());
        return;
    }

    this->directoryList.append(result.oneRow.at(0));
}

void TableCreatorExternalProperties::dirListFetchCompleted(const QString &/*taskName*/)
{
    generalInfo->directoryListAvailable(this->directoryList);
    if(getCreatorMode() != DbObjectCreator::EditExisting){
        fieldProperties->directoryListAvailable(this->directoryList);
    }
}

TableExternalInfo TableCreatorExternalProperties::getExternalInfo() const
{
    TableExternalInfo info=generalInfo->itemInfoFromWidgets();
    if(getCreatorMode() == DbObjectCreator::EditExisting){
        Q_ASSERT(accessParamsEditor);

        info.accessParameters=accessParamsEditor->getAccessParams();
    }else{
        Q_ASSERT(accessParamsEditor==0);

        QString driverPropsDdl;
        if(generalInfo->selectedDriverIndex()==0){
            driverPropsDdl=recordProperties->generateDdl();
            driverPropsDdl.append(fieldProperties->generateDdl());
        }else{
            driverPropsDdl=dataPumpProperties->generateDdl();
        }
        info.accessParameters=driverPropsDdl;
    }
    return info;
}

void TableCreatorExternalProperties::setTableInfo(TableInfo *tableInfo)
{
    generalInfo->setAttributes(&tableInfo->externalInfo);
    accessParamsEditor->setAccessParams(tableInfo->externalInfo.accessParameters);
}

void TableCreatorExternalProperties::alterQuerySucceeded(const QString &taskName)
{
    if(taskName=="alter_external_table_attributes"){

        int projectColumn=generalInfo->originalExternalInfo->projectColumn;
        *generalInfo->originalExternalInfo=generalInfo->itemInfoFromWidgets();
        generalInfo->originalExternalInfo->accessParameters=accessParamsEditor->getAccessParams();
        generalInfo->originalExternalInfo->projectColumn=projectColumn; //project column is updated with separate dml

    }else if(taskName=="alter_external_table_project_column"){

        *generalInfo->originalExternalInfo=generalInfo->itemInfoFromWidgets();
        generalInfo->originalExternalInfo->accessParameters=accessParamsEditor->getAccessParams();

    }
}

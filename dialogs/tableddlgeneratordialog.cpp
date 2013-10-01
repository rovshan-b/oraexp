#include "tableddlgeneratordialog.h"
#include "widgets/tabledbmsddloptionswidget.h"
#include "widgets/tableddlexportoptionswidget.h"
#include "widgets/storageddlexportoptionswidget.h"
#include "util/dialoghelper.h"
#include "util/widgethelper.h"
#include "beans/objectddloptions.h"
#include "metadata_loaders/table/tableinfoloader.h"
#include "code_generators/ddl/tableddlgenerator.h"
#include <QtGui>

TableDdlGeneratorDialog::TableDdlGeneratorDialog(DbUiManager *uiManager,
                                                 const QString &schemaName,
                                                 const QString &tableName,
                                                 QWidget *parent) :
    CodeGeneratorDialog(uiManager, parent),
    schemaName(schemaName),
    tableName(tableName)
{

}

void TableDdlGeneratorDialog::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QFormLayout *form = new QFormLayout();

    methodCombo = new QComboBox();
    methodCombo->addItem(tr("Application"));
    methodCombo->addItem(tr("DBMS_DDL"));
    form->addRow(tr("Method"), methodCombo);

    mainLayout->addLayout(form);

    QStackedWidget *tab = new QStackedWidget();

    QVBoxLayout *appTabLayout = new QVBoxLayout();

    appExpOptionsWidget = new TableDdlExportOptionsWidget(false);
    appTabLayout->addWidget(WidgetHelper::createGroupBox(appExpOptionsWidget, tr("Table options")));

    appStorageOptionsWidget = new StorageDdlExportOptionsWidget();
    appTabLayout->addWidget(WidgetHelper::createGroupBox(appStorageOptionsWidget, tr("Storage options")));

    QWidget *appTabWidget = new QWidget();
    appTabLayout->setContentsMargins(0,0,0,0);
    appTabWidget->setLayout(appTabLayout);
    tab->addWidget(appTabWidget);

    dbmsOptionsWidget = new TableDbmsDdlOptionsWidget();
    tab->addWidget(WidgetHelper::nestWidget(dbmsOptionsWidget, true));

    mainLayout->addWidget(tab);

    createButtonBox();
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    connect(methodCombo, SIGNAL(currentIndexChanged(int)), tab, SLOT(setCurrentIndex(int)));

    resize(sizeHint());
    DialogHelper::centerWindow(this);
}

QString TableDdlGeneratorDialog::generateCode()
{
    int method = methodCombo->currentIndex();

    if(method == 0){ //dbms_ddl
        TableInfoLoader *metadataLoader=new TableInfoLoader(this, schemaName, tableName, this);
        connect(metadataLoader, SIGNAL(objectInfoReady(DbObjectInfo*,MetadataLoader*)), this, SLOT(tableInfoReady(DbObjectInfo*,MetadataLoader*)));
        connect(metadataLoader, SIGNAL(loadError(QString,OciException,MetadataLoader*)), this, SLOT(loadError(QString,OciException,MetadataLoader*)));
        metadataLoader->loadObjectInfo();
    }else{ //application
        QList<Param*> params;
        params << new Param("owner", schemaName) << new Param("object_name", tableName) << new Param("object_type", QString("TABLE"));
        params.append(dbmsOptionsWidget->getOptions().getQueryParams());
        enqueueQuery("get_object_ddl",
                     params,
                     this,
                     "get_object_ddl",
                     "ddlQueryCompleted",
                     "ddlFetched",
                     "ddlFetchCompleted",
                     true);
    }

    return "";
}

void TableDdlGeneratorDialog::setInProgress(bool inProgress)
{
    CodeGeneratorDialog::setInProgress(inProgress);

    this->setEnabled(!inProgress);
}

void TableDdlGeneratorDialog::ddlQueryCompleted(const QueryResult &result)
{
    if(result.hasError){
        QMessageBox::critical(this->window(), tr("Error retrieving table DDL"), result.exception.getErrorMessage());

        setInProgress(false);
    }
}

void TableDdlGeneratorDialog::ddlFetched(const FetchResult &fetchResult)
{
    if(fetchResult.hasError){
        QMessageBox::critical(this->window(), tr("Error fetching table DDL"), fetchResult.exception.getErrorMessage());
        setInProgress(false);
    }else{
        ddl = fetchResult.oneRow.at(0);
    }
}

void TableDdlGeneratorDialog::ddlFetchCompleted(const QString &)
{
    codeReady(ddl);
}

void TableDdlGeneratorDialog::tableInfoReady(DbObjectInfo *objectInfo, MetadataLoader *loader)
{
    TableInfo *tableInfo=static_cast<TableInfo*>(objectInfo);

    NewDbObjectStorageOptions storageOptions = appStorageOptionsWidget->getOptions();
    tableInfo->prepareForOptions(storageOptions);

    TableCreateDdlOptions ddlOptions = appExpOptionsWidget->getOptions();
    ddlOptions.newObjectStorageOptions = storageOptions;

    QPair<QString, QString> codePair = TableDdlGenerator::generateDdlForExporting(*tableInfo, ddlOptions);

    ddl = codePair.first;
    ddl.append("\n").append(codePair.second);

    delete tableInfo;
    loader->deleteLater();

    codeReady(ddl);
}

void TableDdlGeneratorDialog::loadError(const QString &taskName, const OciException &ex, MetadataLoader *loader)
{
    QMessageBox::critical(this, tr("Failed to load table information"),
                          tr("Task name: %1\nError: %2").arg(taskName, ex.getErrorMessage()));

    loader->deleteLater();

    setInProgress(false);
}

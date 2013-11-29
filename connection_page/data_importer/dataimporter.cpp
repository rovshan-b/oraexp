#include "dataimporter.h"
#include "widgets/dbitemlistcombobox.h"
#include "pages/dataimporterfirstpage.h"
#include "pages/dataimportercsvoptionspage.h"
#include "pages/dataimportercolumnmappingspage.h"
#include "pages/dataimporterqueriespage.h"
#include "pages/dataimporterreadypage.h"
#include "pages/dataimporterimportpage.h"
#include "pages/dataimporterworksheetpage.h"
#include "util/iconutil.h"
#include "util/dialoghelper.h"
#include <QtGui>

DataImporter::DataImporter(const QString &schemaName, const QString &tableName, DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageWizard(uiManager, parent),
    schemaName(schemaName),
    tableName(tableName)
{
    setPixmap(QWizard::LogoPixmap, IconUtil::getIcon("import_data"));

    resize(650, 550);
    DialogHelper::centerWindow(this);
}

DataImporter::~DataImporter()
{
    removePage(pageIds().size()-1);
    delete worksheetPage;
}

void DataImporter::setConnection(DbConnection *db)
{
    ConnectionPageWizard::setConnection(db);

    firstPage->setQueryScheduler(this);
    mappingsPage->setQueryScheduler(this);
    importPage->setQueryScheduler(this);
    worksheetPage->setQueryScheduler(this);

    emitInitCompletedSignal();
}

QStandardItemModel *DataImporter::getDataPreviewModel() const
{
    return csvOptionsPage->getDataPreviewModel();
}

CsvImporter *DataImporter::getImporter() const
{
    return csvOptionsPage->getImporter();
}

QList<ColumnMapping *> DataImporter::getColumnMappings() const
{
    return mappingsPage->getColumnMappings();
}

void DataImporter::createUi()
{
    firstPage = new DataImporterFirstPage(this->schemaName,
                                          this->tableName,
                                          uiManager);
    addPage(firstPage);

    csvOptionsPage = new DataImporterCsvOptionsPage(uiManager);
    addPage(csvOptionsPage);

    mappingsPage = new DataImporterColumnMappingsPage(uiManager);
    addPage(mappingsPage);

    queriesPage = new DataImporterQueriesPage(uiManager);
    addPage(queriesPage);

    readyPage = new DataImporterReadyPage(uiManager);
    addPage(readyPage);

    importPage = new DataImporterImportPage(uiManager);
    addPage(importPage);

    worksheetPage = new DataImporterWorksheetPage(uiManager);
    addPage(worksheetPage);

    setOption(QWizard::DisabledBackButtonOnLastPage, true);
}

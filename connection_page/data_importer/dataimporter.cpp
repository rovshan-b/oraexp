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

DataImporter::DataImporter(const QString &schemaName, const QString &tableName, QWidget *parent) :
    ConnectionPageWizard(parent),
    schemaName(schemaName),
    tableName(tableName)
{
    setPixmap(QWizard::LogoPixmap, IconUtil::getIcon("import_data"));

    resize(650, 550);
    DialogHelper::centerWindow(this);
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
                                          this->tableName);
    addPage(firstPage);

    csvOptionsPage = new DataImporterCsvOptionsPage();
    addPage(csvOptionsPage);

    mappingsPage = new DataImporterColumnMappingsPage();
    addPage(mappingsPage);

    queriesPage = new DataImporterQueriesPage();
    addPage(queriesPage);

    readyPage = new DataImporterReadyPage();
    addPage(readyPage);

    importPage = new DataImporterImportPage();
    addPage(importPage);

    worksheetPage = new DataImporterWorksheetPage();
    addPage(worksheetPage);

    setOption(QWizard::DisabledBackButtonOnLastPage, true);
}

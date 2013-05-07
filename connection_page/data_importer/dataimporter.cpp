#include "dataimporter.h"
#include "widgets/dbitemlistcombobox.h"
#include "pages/dataimporterfirstpage.h"
#include "pages/dataimportercsvoptionspage.h"
#include "util/iconutil.h"
#include <QtGui>

DataImporter::DataImporter(const QString &schemaName, const QString &tableName, QWidget *parent) :
    ConnectionPageWizard(parent),
    schemaName(schemaName),
    tableName(tableName)
{
    setPixmap(QWizard::LogoPixmap, IconUtil::getIcon("import_data"));

    resize(600, 500);
}

void DataImporter::setConnection(DbConnection *db)
{
    ConnectionPageWizard::setConnection(db);

    firstPage->setQueryScheduler(this);

    emitInitCompletedSignal();
}

void DataImporter::createUi()
{
    firstPage = new DataImporterFirstPage(this->schemaName,
                                          this->tableName);
    addPage(firstPage);

    csvOptionsPage = new DataImporterCsvOptionsPage();
    addPage(csvOptionsPage);
}

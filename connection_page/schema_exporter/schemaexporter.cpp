#include "schemaexporter.h"
#include "connection_page/db_object_comparer/tabs/dbobjectcomparercomparetab.h"
#include "tabs/schemaexporteroptionstab.h"
#include "connection_page/db_object_comparer/tabs/dbobjectcomparerresultstab.h"

SchemaExporter::SchemaExporter(DbUiManager *uiManager, QWidget *parent) :
    DbObjectComparer(uiManager, parent)
{
}

DbObjectComparerCompareTab *SchemaExporter::doCreateCompareTab(DbUiManager *uiManager)
{
    return new DbObjectComparerCompareTab(uiManager);
}

DbObjectComparerOptionsTab *SchemaExporter::doCreateOptionsTab()
{
    return new SchemaExporterOptionsTab();
}

DbObjectComparerResultsTab *SchemaExporter::doCreateResultsTab()
{
    return new DbObjectComparerResultsTab(this->uiManager);
}

IDbObjectCompareHelper *SchemaExporter::createCompareHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler, const QString &targetSchema, IQueryScheduler *targetScheduler, DbObjectComparisonOptions *options, QObject *parent)
{
    return 0;
}

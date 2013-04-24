#include "schemaexporter.h"
#include "tabs/schemaexportercomparetab.h"
#include "tabs/schemaexporteroptionstab.h"
#include "connection_page/db_object_comparer/tabs/dbobjectcomparerresultstab.h"
#include "schemaexporterhelper.h"
#include "beans/schemaexportoptions.h"

SchemaExporter::SchemaExporter(DbUiManager *uiManager, QWidget *parent) :
    DbObjectComparer(uiManager, parent)
{
}

DbObjectComparerCompareTab *SchemaExporter::doCreateCompareTab(DbUiManager *uiManager)
{
    return new SchemaExporterCompareTab(uiManager);
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
    Q_UNUSED(targetSchema);
    Q_UNUSED(targetScheduler);

    SchemaExporterHelper *exporter = new SchemaExporterHelper(sourceSchema, sourceScheduler,
                                                              compareTab->getObjectsModel(),
                                                              static_cast<SchemaExportOptions*>(options),
                                                              parent);

    //connect(exporter, SIGNAL(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)), resultsTab, SLOT(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)));
    connect(exporter, SIGNAL(statusChanged(QString)), this, SLOT(statusChanged(QString)));
    //connect(exporter, SIGNAL(ddlReady(QString)), this, SLOT(comparisonResultAvailable(QString)));
    connect(exporter, SIGNAL(completed()), this, SLOT(completed()));
    connect(exporter, SIGNAL(comparisonError(QString,OciException)), this, SLOT(comparisonError(QString,OciException)));
    connect(exporter, SIGNAL(objectCountDetermined(int)), this, SLOT(objectCountDetermined(int)));
    connect(exporter, SIGNAL(chunkCompleted(int)), this, SLOT(chunkCompleted(int)));

    return exporter;
}

void SchemaExporter::createTabs()
{
    DbObjectComparer::createTabs();

    SchemaExporterOptionsTab *optTab = static_cast<SchemaExporterOptionsTab*>(optionsTab);
    optTab->createFileWidgets(compareTab->getTopPaneForm());
}

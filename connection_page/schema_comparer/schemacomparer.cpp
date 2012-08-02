#include "schemacomparer.h"
#include "tabs/schemacomparercomparetab.h"
#include "tabs/schemacompareroptionstab.h"
#include "tabs/schemacomparerresultstab.h"
#include "widgets/subtabwidget.h"
#include "comparers/schemacomparerhelper.h"
#include "connectivity/dbconnection.h"
#include "util/strutil.h"
#include <QtGui>

SchemaComparer::SchemaComparer(DbUiManager *uiManager, QWidget *parent) :
    DbObjectComparer(uiManager, parent)
{

}

DbObjectComparerCompareTab *SchemaComparer::doCreateCompareTab(DbUiManager *uiManager)
{
    return new SchemaComparerCompareTab(uiManager, this);
}

DbObjectComparerOptionsTab *SchemaComparer::doCreateOptionsTab()
{
    return new SchemaComparerOptionsTab(this);
}

DbObjectComparerResultsTab *SchemaComparer::doCreateResultsTab()
{
    return new SchemaComparerResultsTab(this);
}

IDbObjectCompareHelper *SchemaComparer::createCompareHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                                            const QString &targetSchema, IQueryScheduler *targetScheduler,
                                                            DbObjectComparisonOptions *options, QObject *parent)
{
    SchemaComparerHelper *comparer=new SchemaComparerHelper(sourceSchema,
                                      sourceScheduler,
                                      targetSchema,
                                      targetScheduler,
                                      compareTab->getObjectsModel(),
                                      static_cast<SchemaComparisonOptions*>(options),
                                      parent);

    connect(comparer, SIGNAL(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)), resultsTab, SLOT(changedObjectDetected(QString,DbTreeModel::DbTreeNodeType,QString)));
    connect(comparer, SIGNAL(statusChanged(QString)), this, SLOT(statusChanged(QString)));
    connect(comparer, SIGNAL(ddlReady(QString)), this, SLOT(comparisonResultAvailable(QString)));
    connect(comparer, SIGNAL(completed()), this, SLOT(completed()));
    connect(comparer, SIGNAL(comparisonError(QString,OciException)), this, SLOT(comparisonError(QString,OciException)));
    connect(comparer, SIGNAL(objectCountDetermined(int)), this, SLOT(objectCountDetermined(int)));
    connect(comparer, SIGNAL(chunkCompleted(int)), this, SLOT(chunkCompleted(int)));

    return comparer;
}

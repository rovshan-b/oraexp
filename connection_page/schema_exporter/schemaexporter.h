#ifndef SCHEMAEXPORTER_H
#define SCHEMAEXPORTER_H

#include "connection_page/db_object_comparer/dbobjectcomparer.h"

class SchemaExporter : public DbObjectComparer
{
    Q_OBJECT
public:
    explicit SchemaExporter(DbUiManager *uiManager, QWidget *parent = 0);

protected:
    virtual DbObjectComparerCompareTab *doCreateCompareTab(DbUiManager *uiManager);
    virtual DbObjectComparerOptionsTab *doCreateOptionsTab();
    virtual DbObjectComparerResultsTab *doCreateResultsTab();

    virtual IDbObjectCompareHelper *createCompareHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                                        const QString &targetSchema, IQueryScheduler *targetScheduler,
                                                        DbObjectComparisonOptions *options, QObject *parent);
    
protected slots:
    virtual void createTabs();
    
};

#endif // SCHEMAEXPORTER_H

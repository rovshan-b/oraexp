#ifndef SCHEMACOMPARER_H
#define SCHEMACOMPARER_H

#include "connection_page/db_object_comparer/dbobjectcomparer.h"

class SchemaComparer : public DbObjectComparer
{
    Q_OBJECT
public:
    explicit SchemaComparer(DbUiManager *uiManager, QWidget *parent = 0);

protected:
    virtual DbObjectComparerCompareTab* doCreateCompareTab(DbUiManager *uiManager);
    virtual DbObjectComparerOptionsTab *doCreateOptionsTab();
    virtual DbObjectComparerResultsTab *doCreateResultsTab();
    virtual IDbObjectCompareHelper *createCompareHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                                        const QString &targetSchema, IQueryScheduler *targetScheduler,
                                                        DbObjectComparisonOptions *options, QObject *parent);
};

#endif // SCHEMACOMPARER_H

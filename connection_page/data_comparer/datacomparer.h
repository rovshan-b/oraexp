#ifndef DATACOMPARER_H
#define DATACOMPARER_H

#include "../connectionpagetab.h"
#include "connection_page/db_object_comparer/dbobjectcomparer.h"

class DataComparer : public DbObjectComparer
{
    Q_OBJECT
public:
    explicit DataComparer(DbUiManager *uiManager, QWidget *parent = 0);

protected:
    virtual DbObjectComparerCompareTab* doCreateCompareTab(DbUiManager *uiManager);
    virtual DbObjectComparerOptionsTab *doCreateOptionsTab();
    virtual DbObjectComparerResultsTab *doCreateResultsTab();
    virtual IDbObjectCompareHelper *createCompareHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                                        const QString &targetSchema, IQueryScheduler *targetScheduler,
                                                        DbObjectComparisonOptions *options, QObject *parent);

    void connectComparerSignalsAndSlots(QObject *comparer);

protected slots:
    void tableCompareSuccess(const QString &tableName);

};

#endif // DATACOMPARER_H

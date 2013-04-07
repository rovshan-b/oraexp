#ifndef DATAEXPORTER_H
#define DATAEXPORTER_H

#include "connection_page/data_comparer/datacomparer.h"

class DataExporter : public DataComparer
{
    Q_OBJECT
public:
    explicit DataExporter(DbUiManager *uiManager, QWidget *parent = 0);
    
    virtual DbObjectComparerCompareTab* doCreateCompareTab(DbUiManager *uiManager);
    virtual DbObjectComparerOptionsTab *doCreateOptionsTab();
    virtual DbObjectComparerResultsTab *doCreateResultsTab();
    virtual IDbObjectCompareHelper *createCompareHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                                        const QString &targetSchema, IQueryScheduler *targetScheduler,
                                                        DbObjectComparisonOptions *options, QObject *parent);

protected slots:
    virtual void createTabs();
    
};

#endif // DATAEXPORTER_H

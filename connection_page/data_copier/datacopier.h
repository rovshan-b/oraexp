#ifndef DATACOPIER_H
#define DATACOPIER_H

#include "connection_page/data_comparer/datacomparer.h"

class DataCopier : public DataComparer
{
    Q_OBJECT
public:
    explicit DataCopier(DbUiManager *uiManager, QWidget *parent = 0);
    
protected:
    virtual DbObjectComparerOptionsTab *doCreateOptionsTab();
    virtual DbObjectComparerResultsTab *doCreateResultsTab();
    virtual IDbObjectCompareHelper *createCompareHelper(const QString &sourceSchema, IQueryScheduler *sourceScheduler,
                                                        const QString &targetSchema, IQueryScheduler *targetScheduler,
                                                        DbObjectComparisonOptions *options, QObject *parent);
    
};

#endif // DATACOPIER_H

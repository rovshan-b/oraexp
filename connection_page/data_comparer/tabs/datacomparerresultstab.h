#ifndef DATACOMPARERRESULTSTAB_H
#define DATACOMPARERRESULTSTAB_H

#include "connection_page/db_object_comparer/tabs/dbobjectcomparerresultstab.h"
#include "beans/datacompareinfo.h"

class DataComparerResultsTab : public DbObjectComparerResultsTab
{
    Q_OBJECT
public:
    explicit DataComparerResultsTab(DbUiManager *uiManager, QWidget *parent = 0);

    virtual DataTable *createChangeListTable();
    
signals:
    
public slots:
    virtual void compareInfoAvailable(const DataCompareInfo &info);
    virtual void setCurrentSchema(const QString &currentSchemaName);

protected:
    virtual QStringList getTableColumnNames() const;
    virtual void updateDmlCounts(int row, const DataCompareInfo &info);
    
};

#endif // DATACOMPARERRESULTSTAB_H

#ifndef DATAEXPORTERRESULTSTAB_H
#define DATAEXPORTERRESULTSTAB_H

#include "connection_page/data_comparer/tabs/datacomparerresultstab.h"
#include "beans/datacompareinfo.h"

class DataExporterResultsTab : public DataComparerResultsTab
{
    Q_OBJECT
public:
    explicit DataExporterResultsTab(DbUiManager *uiManager, QWidget *parent = 0);
    
protected:
    virtual QStringList getTableColumnNames() const;
    virtual void updateDmlCounts(int row, const DataCompareInfo &info);
    
};

#endif // DATAEXPORTERRESULTSTAB_H

#ifndef DATACOPIERRESULTSTAB_H
#define DATACOPIERRESULTSTAB_H

#include "connection_page/data_comparer/tabs/datacomparerresultstab.h"
#include "beans/datacompareinfo.h"

class DataCopierResultsTab : public DataComparerResultsTab
{
    Q_OBJECT
public:
    explicit DataCopierResultsTab(DbUiManager *uiManager, QWidget *parent = 0);
    
protected:
    virtual QStringList getTableColumnNames() const;
};

#endif // DATACOPIERRESULTSTAB_H

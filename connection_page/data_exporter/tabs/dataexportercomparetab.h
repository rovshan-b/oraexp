#ifndef DATAEXPORTERCOMPARETAB_H
#define DATAEXPORTERCOMPARETAB_H

#include "connection_page/data_comparer/tabs/datacomparercomparetab.h"

class DataExporterCompareTab : public DataComparerCompareTab
{
    Q_OBJECT
public:
    explicit DataExporterCompareTab(DbUiManager *uiManager, QWidget *parent);

    virtual bool needsTargetConnection() const;

    
};

#endif // DATAEXPORTERCOMPARETAB_H

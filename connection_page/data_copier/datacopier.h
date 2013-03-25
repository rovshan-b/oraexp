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
    
};

#endif // DATACOPIER_H

#ifndef DATACOPIEROPTIONSTAB_H
#define DATACOPIEROPTIONSTAB_H

#include "connection_page/db_object_comparer/tabs/dbobjectcompareroptionstab.h"

class QComboBox;

class DataCopierOptionsTab : public DbObjectComparerOptionsTab
{
    Q_OBJECT
public:
    explicit DataCopierOptionsTab(QWidget *parent = 0);

    virtual DbObjectComparisonOptions *getOptions() const;
    
private:
    void createDataCopierOptionsPane(QVBoxLayout *layout);

    QComboBox *truncateBeforeCopyComboBox;
    QComboBox *disableRefContraintsComboBox;
    
};

#endif // DATACOPIEROPTIONSTAB_H

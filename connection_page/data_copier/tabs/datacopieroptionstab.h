#ifndef DATACOPIEROPTIONSTAB_H
#define DATACOPIEROPTIONSTAB_H

#include "connection_page/data_operation/tabs/dataoperationoptionstab.h"

class QComboBox;
class QCheckBox;

class DataCopierOptionsTab : public DataOperationOptionsTab
{
    Q_OBJECT
public:
    explicit DataCopierOptionsTab(QWidget *parent = 0);

    virtual DbObjectComparisonOptions *getOptions() const;
    
private slots:
    void truncateOptionChanged();

private:
    void createOptionsPane(QVBoxLayout *layout);

    QCheckBox *truncateBeforeCopyCheckBox;
    
};

#endif // DATACOPIEROPTIONSTAB_H

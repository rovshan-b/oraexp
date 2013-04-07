#ifndef DATACOMPAREROPTIONSTAB_H
#define DATACOMPAREROPTIONSTAB_H

#include "connection_page/data_operation/tabs/dataoperationoptionstab.h"

class QBoxLayout;
class QVBoxLayout;
class QCheckBox;
class QComboBox;

class DataComparerOptionsTab : public DataOperationOptionsTab
{
    Q_OBJECT
public:
    DataComparerOptionsTab(QWidget *parent = 0);

    virtual DbObjectComparisonOptions *getOptions();

private slots:
    void enableControls();
    void deletesCheckboxChanged();

protected:
    QCheckBox *insertsCheckbox;
    QCheckBox *updatesCheckbox;
    QCheckBox *deletesCheckbox;
    QCheckBox *includeSchemaCheckBox;
    QComboBox *comparisonModeComboBox;

    virtual void createOptionsPane(QVBoxLayout *layout);
};

#endif // DATACOMPAREROPTIONSTAB_H

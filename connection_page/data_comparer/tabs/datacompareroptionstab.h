#ifndef DATACOMPAREROPTIONSTAB_H
#define DATACOMPAREROPTIONSTAB_H

#include "connection_page/db_object_comparer/tabs/dbobjectcompareroptionstab.h"

class QBoxLayout;
class QVBoxLayout;
class QCheckBox;
class QComboBox;

class DataComparerOptionsTab : public DbObjectComparerOptionsTab
{
    Q_OBJECT
public:
    DataComparerOptionsTab(QWidget *parent = 0);

    virtual DbObjectComparisonOptions *getOptions() const;

private slots:
    void enableControls();

private:
    QCheckBox *insertsCheckbox;
    QCheckBox *updatesCheckbox;
    QCheckBox *deletesCheckbox;
    QCheckBox *disableRefContraintsCheckBox;
    QCheckBox *includeSchemaCheckBox;
    QComboBox *comparisonModeComboBox;

    void createOptionsPane(QBoxLayout *layout);
    void createDataCompareOptionsPane(QVBoxLayout *layout);
};

#endif // DATACOMPAREROPTIONSTAB_H

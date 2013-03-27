#ifndef DATAOPERATIONOPTIONSTAB_H
#define DATAOPERATIONOPTIONSTAB_H

#include "connection_page/db_object_comparer/tabs/dbobjectcompareroptionstab.h"
#include "beans/dataoperationoptions.h"

class QCheckBox;
class QComboBox;
class QVBoxLayout;
class QFormLayout;

class DataOperationOptionsTab : public DbObjectComparerOptionsTab
{
    Q_OBJECT
public:
    explicit DataOperationOptionsTab(QWidget *parent = 0);

    virtual void createUi();

    virtual DbObjectComparisonOptions *getOptions() const = 0;
    
protected:
    QComboBox *disableRefContraintsComboBox;
    QCheckBox *skipOnErrorCheckBox;
    QCheckBox *uncheckInGuiCheckBox;

    virtual void createOptionsPane(QVBoxLayout *layout) = 0;
    void setOptions(DataOperationOptions *options) const;

    void addDisableRefConstraintsComboBox(QFormLayout *form, int currentIndex);
    void addSkipOnErrorCheckBox(QFormLayout *form);
    void addUncheckInGuiCheckBox(QFormLayout *form);
    
};

#endif // DATAOPERATIONOPTIONSTAB_H

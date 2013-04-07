#include "datacopieroptionstab.h"
#include "beans/datacopieroptions.h"
#include "util/widgethelper.h"
#include <QtGui>

DataCopierOptionsTab::DataCopierOptionsTab(QWidget *parent) :
    DataOperationOptionsTab(parent)
{
}

DbObjectComparisonOptions *DataCopierOptionsTab::getOptions()
{
    DataCopierOptions *options = new DataCopierOptions();

    options->comparisonMode = DataOperationOptions::UpdateDatabase;
    options->truncate = truncateBeforeCopyCheckBox->isChecked();

    setOptions(options);

    return options;
}

void DataCopierOptionsTab::truncateOptionChanged()
{
    if(truncateBeforeCopyCheckBox->isChecked()){
        disableRefContraintsComboBox->setCurrentIndex(DataOperationOptions::DisableForAll);
    }
}

void DataCopierOptionsTab::createOptionsPane(QVBoxLayout *layout)
{
    QFormLayout *form = new QFormLayout();

    truncateBeforeCopyCheckBox = new QCheckBox(tr("Truncate before copy"));
    form->addRow(truncateBeforeCopyCheckBox);

    addSkipOnErrorCheckBox(form);
    addUncheckInGuiCheckBox(form);

    addDisableRefConstraintsComboBox(form, DataOperationOptions::Auto);

    QGroupBox *groupBox = WidgetHelper::createGroupBox(form, tr("Options"));
    layout->addWidget(groupBox);
    layout->setAlignment(groupBox, Qt::AlignTop|Qt::AlignLeft);

    connect(truncateBeforeCopyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(truncateOptionChanged()));
}

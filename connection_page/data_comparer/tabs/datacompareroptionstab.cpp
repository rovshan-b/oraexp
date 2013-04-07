#include "datacompareroptionstab.h"
#include "beans/datacomparisonoptions.h"
#include "util/widgethelper.h"
#include <QtGui>

DataComparerOptionsTab::DataComparerOptionsTab(QWidget *parent) :
    DataOperationOptionsTab(parent)
{

}

DbObjectComparisonOptions *DataComparerOptionsTab::getOptions()
{
    DataComparisonOptions *options=new DataComparisonOptions();

    options->inserts=insertsCheckbox->isChecked();
    options->updates=updatesCheckbox->isChecked();
    options->deletes=deletesCheckbox->isChecked();
    options->includeSchemaName=includeSchemaCheckBox->isChecked();
    options->comparisonMode=comparisonModeComboBox->currentIndex() == 0 ? DataComparisonOptions::GenerateDml : DataComparisonOptions::UpdateDatabase;

    setOptions(options);

    return options;
}

void DataComparerOptionsTab::createOptionsPane(QVBoxLayout *layout)
{
    QFormLayout *form = new QFormLayout();

    insertsCheckbox = WidgetHelper::createCheckBox(form, tr("Generate INSERT statements"), true);
    updatesCheckbox = WidgetHelper::createCheckBox(form, tr("Generate UPDATE statements"), true);
    deletesCheckbox = WidgetHelper::createCheckBox(form, tr("Generate DELETE statements"), true);
    includeSchemaCheckBox = WidgetHelper::createCheckBox(form, tr("Include schema"), true);

    addSkipOnErrorCheckBox(form);
    addUncheckInGuiCheckBox(form);

    addDisableRefConstraintsComboBox(form, DataOperationOptions::Disable);

    comparisonModeComboBox = new QComboBox();
    comparisonModeComboBox->addItem(tr("Generate script"));
    comparisonModeComboBox->addItem(tr("Update target database"));
    comparisonModeComboBox->setCurrentIndex(0);
    form->addRow(tr("Comparison mode"), comparisonModeComboBox);

    QGroupBox *groupBox = WidgetHelper::createGroupBox(form, tr("Options"));
    layout->addWidget(groupBox);
    layout->setAlignment(groupBox, Qt::AlignTop|Qt::AlignLeft);

    enableControls();
    connect(comparisonModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableControls()));
    connect(deletesCheckbox, SIGNAL(stateChanged(int)), this, SLOT(deletesCheckboxChanged()));
}

void DataComparerOptionsTab::enableControls()
{
    includeSchemaCheckBox->setEnabled(comparisonModeComboBox->currentIndex()==DataComparisonOptions::GenerateDml);
}


void DataComparerOptionsTab::deletesCheckboxChanged()
{
    if(deletesCheckbox->isChecked()){
        disableRefContraintsComboBox->setCurrentIndex(DataComparisonOptions::Disable);
    }
}

#include "dataoperationoptionstab.h"
#include <QtGui>

DataOperationOptionsTab::DataOperationOptionsTab(QWidget *parent) :
    DbObjectComparerOptionsTab(parent),
    disableRefContraintsComboBox(0),
    skipOnErrorCheckBox(0),
    uncheckInGuiCheckBox(0)
{
}

void DataOperationOptionsTab::createUi()
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    createOptionsPane(createSingleColumnOptionsPane(mainLayout));

    setLayout(mainLayout);
}

void DataOperationOptionsTab::setOptions(DataOperationOptions *options) const
{
    options->disableRefConstraints = disableRefContraintsComboBox ? (DataOperationOptions::DisableRefConstraintsMode)disableRefContraintsComboBox->currentIndex() : DataOperationOptions::DoNotDisable;
    options->skipOnError = skipOnErrorCheckBox ? skipOnErrorCheckBox->isChecked() : false;
    options->uncheckInGuiOnSuccess = uncheckInGuiCheckBox ? uncheckInGuiCheckBox->isChecked() : false;
}

void DataOperationOptionsTab::addDisableRefConstraintsComboBox(QFormLayout *form, int currentIndex)
{
    disableRefContraintsComboBox = new QComboBox();
    disableRefContraintsComboBox->addItem(tr("Auto"));
    disableRefContraintsComboBox->addItem(tr("Disable for selected tables"));
    disableRefContraintsComboBox->addItem(tr("Disable for all tables"));
    disableRefContraintsComboBox->addItem(tr("Do not disable"));

    disableRefContraintsComboBox->setCurrentIndex(currentIndex);

    form->addRow(tr("Disable referential constraints"), disableRefContraintsComboBox);
}

void DataOperationOptionsTab::addSkipOnErrorCheckBox(QFormLayout *form)
{
    skipOnErrorCheckBox = new QCheckBox(tr("On error skip to next table"));
    form->addRow(skipOnErrorCheckBox);
}

void DataOperationOptionsTab::addUncheckInGuiCheckBox(QFormLayout *form)
{
    uncheckInGuiCheckBox = new QCheckBox(tr("On success uncheck in tree view"));
    //uncheckInGuiCheckBox->setChecked(true);
    form->addRow(uncheckInGuiCheckBox);
}

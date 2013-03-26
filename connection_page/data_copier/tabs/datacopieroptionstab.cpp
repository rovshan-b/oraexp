#include "datacopieroptionstab.h"
#include "beans/datacopieroptions.h"
#include <QtGui>

DataCopierOptionsTab::DataCopierOptionsTab(QWidget *parent) :
    DbObjectComparerOptionsTab(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    createDataCopierOptionsPane(createSingleColumnOptionsPane(mainLayout));

    setLayout(mainLayout);
}

DbObjectComparisonOptions *DataCopierOptionsTab::getOptions() const
{
    DataCopierOptions *options = new DataCopierOptions();

    options->comparisonMode = DataOperationOptions::UpdateDatabase;
    options->truncate = truncateBeforeCopyCheckBox->isChecked();
    options->disableRefConstraints = (DataOperationOptions::DisableRefConstraintsMode)disableRefContraintsComboBox->currentIndex();

    return options;
}

void DataCopierOptionsTab::truncateOptionChanged()
{
    if(truncateBeforeCopyCheckBox->isChecked()){
        disableRefContraintsComboBox->setCurrentIndex(DataOperationOptions::DisableForAll);
    }
}

void DataCopierOptionsTab::createDataCopierOptionsPane(QVBoxLayout *layout)
{
    QGroupBox *dataCompareOptionsGroupBox=new QGroupBox(tr("Options"));
    QGridLayout *grid = new QGridLayout();

    truncateBeforeCopyCheckBox = new QCheckBox(tr("Truncate before copy"));
    grid->addWidget(truncateBeforeCopyCheckBox, 0, 0);

    grid->addWidget(new QLabel(tr("Disable referential constraints")), 1, 0);
    disableRefContraintsComboBox = new QComboBox();
    disableRefContraintsComboBox->addItem(tr("Auto"));
    disableRefContraintsComboBox->addItem(tr("Disable for selected tables"));
    disableRefContraintsComboBox->addItem(tr("Disable for all tables"));
    disableRefContraintsComboBox->addItem(tr("Do not disable"));
    disableRefContraintsComboBox->setCurrentIndex(0);
    grid->addWidget(disableRefContraintsComboBox, 1, 1);

    dataCompareOptionsGroupBox->setLayout(grid);
    layout->addWidget(dataCompareOptionsGroupBox);
    layout->setAlignment(dataCompareOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);

    connect(truncateBeforeCopyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(truncateOptionChanged()));
}

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
    options->truncate = (DataCopierOptions::TruncateMode)truncateBeforeCopyComboBox->currentIndex();
    options->disableRefConstraints = (DataOperationOptions::DisableRefConstraintsMode)disableRefContraintsComboBox->currentIndex();

    return options;
}

void DataCopierOptionsTab::createDataCopierOptionsPane(QVBoxLayout *layout)
{
    QGroupBox *dataCompareOptionsGroupBox=new QGroupBox(tr("Options"));
    QGridLayout *grid = new QGridLayout();

    grid->addWidget(new QLabel(tr("Truncate before copy")), 0, 0);
    truncateBeforeCopyComboBox = new QComboBox();
    truncateBeforeCopyComboBox->addItem(tr("Yes"));
    truncateBeforeCopyComboBox->addItem(tr("Yes & Cascade constraints"));
    truncateBeforeCopyComboBox->addItem(tr("No"));
    truncateBeforeCopyComboBox->setCurrentIndex(2);
    grid->addWidget(truncateBeforeCopyComboBox, 0, 1);

    grid->addWidget(new QLabel(tr("Disable referential constraints")), 1, 0);
    disableRefContraintsComboBox = new QComboBox();
    disableRefContraintsComboBox->addItem(tr("Auto"));
    disableRefContraintsComboBox->addItem(tr("Yes"));
    disableRefContraintsComboBox->addItem(tr("No"));
    disableRefContraintsComboBox->setCurrentIndex(0);
    grid->addWidget(disableRefContraintsComboBox, 1, 1);

    dataCompareOptionsGroupBox->setLayout(grid);
    layout->addWidget(dataCompareOptionsGroupBox);
    layout->setAlignment(dataCompareOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);
}

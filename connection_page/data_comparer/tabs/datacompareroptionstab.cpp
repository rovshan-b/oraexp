#include "datacompareroptionstab.h"
#include "beans/datacomparisonoptions.h"
#include "util/widgethelper.h"
#include <QtGui>

DataComparerOptionsTab::DataComparerOptionsTab(QWidget *parent) : DbObjectComparerOptionsTab(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    createOptionsPane(mainLayout);

    enableControls();

    setLayout(mainLayout);
}

DbObjectComparisonOptions *DataComparerOptionsTab::getOptions() const
{
    DataComparisonOptions *options=new DataComparisonOptions();
    options->inserts=insertsCheckbox->isChecked();
    options->updates=updatesCheckbox->isChecked();
    options->deletes=deletesCheckbox->isChecked();
    options->disableRefConstraints=(DataComparisonOptions::DisableRefConstraintsMode)disableRefContraintsComboBox->currentIndex();
    options->includeSchemaName=includeSchemaCheckBox->isChecked();
    options->comparisonMode=comparisonModeComboBox->currentIndex() == 0 ? DataComparisonOptions::GenerateDml : DataComparisonOptions::UpdateDatabase;

    return options;
}

void DataComparerOptionsTab::createOptionsPane(QBoxLayout *layout)
{
    QVBoxLayout *optionsPaneLayout=new QVBoxLayout();

    QVBoxLayout *firstColLayout=new QVBoxLayout();
    firstColLayout->setContentsMargins(0,0,0,0);

    createDataCompareOptionsPane(firstColLayout);

    optionsPaneLayout->addLayout(firstColLayout);
    optionsPaneLayout->addStretch();

    QWidget *scrollWidget=new QWidget();
    scrollWidget->setContentsMargins(0,0,0,0);
    scrollWidget->setLayout(optionsPaneLayout);

    QScrollArea *scrollArea=new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setBackgroundRole(QPalette::Base);
    scrollArea->setWidget(scrollWidget);

    layout->addWidget(scrollArea);
    layout->setAlignment(optionsPaneLayout, Qt::AlignTop|Qt::AlignLeft);
}

void DataComparerOptionsTab::createDataCompareOptionsPane(QVBoxLayout *layout)
{
    QGroupBox *dataCompareOptionsGroupBox=new QGroupBox(tr("Options"));
    QGridLayout *grid = new QGridLayout();

    insertsCheckbox = WidgetHelper::createCheckBox(grid, 0, 0, tr("Generate INSERT statements"), true, 1, 2);
    updatesCheckbox = WidgetHelper::createCheckBox(grid, 1, 0, tr("Generate UPDATE statements"), true, 1, 2);
    deletesCheckbox = WidgetHelper::createCheckBox(grid, 2, 0, tr("Generate DELETE statements"), true, 1, 2);
    includeSchemaCheckBox = WidgetHelper::createCheckBox(grid, 3, 0, tr("Include schema"), true, 1, 2);

    grid->addWidget(new QLabel(tr("Disable referential constraints")), 4, 0);
    disableRefContraintsComboBox = new QComboBox();
    disableRefContraintsComboBox->addItem(tr("Auto"));
    disableRefContraintsComboBox->addItem(tr("Yes"));
    disableRefContraintsComboBox->addItem(tr("No"));
    disableRefContraintsComboBox->setCurrentIndex(0);
    grid->addWidget(disableRefContraintsComboBox, 4, 1);

    grid->addWidget(new QLabel(tr("Comparison mode")), 5, 0);

    comparisonModeComboBox = new QComboBox();
    comparisonModeComboBox->addItem(tr("Generate script"));
    comparisonModeComboBox->addItem(tr("Update target database"));
    comparisonModeComboBox->setCurrentIndex(0);
    grid->addWidget(comparisonModeComboBox, 5, 1);

    dataCompareOptionsGroupBox->setLayout(grid);
    layout->addWidget(dataCompareOptionsGroupBox);
    layout->setAlignment(dataCompareOptionsGroupBox, Qt::AlignTop|Qt::AlignLeft);

    connect(comparisonModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableControls()));
}

void DataComparerOptionsTab::enableControls()
{
    includeSchemaCheckBox->setEnabled(comparisonModeComboBox->currentIndex()==DataComparisonOptions::GenerateDml);
}

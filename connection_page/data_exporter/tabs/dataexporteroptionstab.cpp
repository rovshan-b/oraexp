#include "dataexporteroptionstab.h"
#include "util/widgethelper.h"
#include <QtGui>

DataExporterOptionsTab::DataExporterOptionsTab(QWidget *parent) :
    DataOperationOptionsTab(parent)
{
}

DbObjectComparisonOptions *DataExporterOptionsTab::getOptions() const
{
    return 0;
}

void DataExporterOptionsTab::createOptionsPane(QVBoxLayout *layout)
{
    optionsWidget = new DataExportOptionsWidget(false);
    optionsWidget->setMultiTableMode();
    layout->addWidget(optionsWidget);
    layout->setAlignment(optionsWidget, Qt::AlignTop|Qt::AlignLeft);

    QFormLayout *form = new QFormLayout();

    singleFileCheckBox = new QCheckBox(tr("Single file"));
    singleFileCheckBox->setChecked(true);
    form->addRow(singleFileCheckBox);

    addSkipOnErrorCheckBox(form);
    addUncheckInGuiCheckBox(form);

    QGroupBox *groupBox = WidgetHelper::createGroupBox(form, tr("Misc"));
    layout->addWidget(groupBox);
    layout->setAlignment(groupBox, Qt::AlignTop|Qt::AlignLeft);

    connect(optionsWidget, SIGNAL(exportFormatChanged(DataExporterBase::ExportFormat)), this, SLOT(exportFormatChanged(DataExporterBase::ExportFormat)));
}

void DataExporterOptionsTab::exportFormatChanged(DataExporterBase::ExportFormat newFormat)
{
    singleFileCheckBox->setEnabled(newFormat==DataExporterBase::Insert);
}

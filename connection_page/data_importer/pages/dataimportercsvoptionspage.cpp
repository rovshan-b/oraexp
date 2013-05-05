#include "dataimportercsvoptionspage.h"
#include "widgets/datatable.h"
#include "util/widgethelper.h"
#include <QtGui>

DataImporterCsvOptionsPage::DataImporterCsvOptionsPage(QWidget *parent) :
    QWizardPage(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    createForm(mainLayout);

    previewTable = new DataTable();
    previewTable->setMinimumHeight(150);
    mainLayout->addWidget(WidgetHelper::createGroupBox(previewTable, tr("Preview")));

    setLayout(mainLayout);
}

void DataImporterCsvOptionsPage::createForm(QVBoxLayout *mainLayout)
{
    QHBoxLayout *hBox = new QHBoxLayout();

    QFormLayout *form1 = new QFormLayout();

    fileEncodingComboBox = new QComboBox();
    form1->addRow(tr("File encoding"), fileEncodingComboBox);

    form1->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(form1);

    QFormLayout *form2 = new QFormLayout();

    delimiterComboBox = new QComboBox();
    form2->addRow(tr("Field delimiter"), delimiterComboBox);

    skipRowsSpinBox = new QSpinBox();
    form2->addRow(tr("Skip rows"), skipRowsSpinBox);

    QFormLayout *form3 = new QFormLayout();

    headerComboBox = new QComboBox();
    form3->addRow(tr("Header"), headerComboBox);

    enclosureEditor = new QLineEdit();
    form3->addRow(tr("Fields enclosed by"), enclosureEditor);

    form2->setContentsMargins(0,0,0,0);
    form3->setContentsMargins(0,0,0,0);
    hBox->addLayout(form2);
    hBox->addLayout(form3);

    hBox->setStretchFactor(form2, 1);
    hBox->setStretchFactor(form3, 1);

    mainLayout->addWidget(WidgetHelper::createGroupBox(hBox, tr("Format options")));
}

#include "dataimportercsvoptionspage.h"
#include "widgets/datatable.h"
#include "util/widgethelper.h"
#include <QDebug>
#include <QtGui>

DataImporterCsvOptionsPage::DataImporterCsvOptionsPage(QWidget *parent) :
    ConnectionPageWizardPage(parent)
{
    setTitle(tr("Format options"));
    setSubTitle(tr("Make sure data displayed in grid is in correct format"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    createForm(mainLayout);

    previewTable = new DataTable();
    previewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(previewTable);

    setLayout(mainLayout);


    connect(fileEncodingComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setEncoding()));
    connect(delimiterComboBox->lineEdit(), SIGNAL(editingFinished()), this, SLOT(setDelimiter()));
    connect(enclosureEditor, SIGNAL(editingFinished()), this, SLOT(setEnclosure()));
    connect(skipRowsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setSkipRows()));
    connect(headerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setHeaderOption()));
    connect(backslashAsEscapeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(setBackslashAsEscape()));
    connect(nullIfEditor, SIGNAL(editingFinished()), this, SLOT(setNullIf()));
}

void DataImporterCsvOptionsPage::initializePage()
{
    setEncoding(false);
    setDelimiter(false);
    setEnclosure(false);
    setSkipRows(false);
    setHeaderOption(false);
    setBackslashAsEscape(false);
    setNullIf(false);
    importer.setFilename(field("fileName").toString());

    previewData();
}

void DataImporterCsvOptionsPage::setEncoding(bool refreshData)
{
    bool set = importer.setEncoding(fileEncodingComboBox->currentText());

    if(set && refreshData){
        previewData();
    }
}

void DataImporterCsvOptionsPage::setDelimiter(bool refreshData)
{
    bool set = importer.setDelimiter(WidgetHelper::getComboBoxUserDataOrText(delimiterComboBox));

    if(set && refreshData){
        previewData();
    }
}

void DataImporterCsvOptionsPage::setEnclosure(bool refreshData)
{
    bool set = importer.setEnclosures(enclosureEditor->text().trimmed().split(',', QString::SkipEmptyParts));

    if(set && refreshData){
        previewData();
    }
}

void DataImporterCsvOptionsPage::setSkipRows(bool refreshData)
{
    bool set = importer.setSkipRows(skipRowsSpinBox->value());

    if(set && refreshData){
        previewData();
    }
}

void DataImporterCsvOptionsPage::setHeaderOption(bool refreshData)
{
    bool set = importer.setHeaderOption((CsvImporter::HeaderOption)headerComboBox->currentIndex());

    if(set && refreshData){
        previewData();
    }
}

void DataImporterCsvOptionsPage::setBackslashAsEscape(bool refreshData)
{
    bool set = importer.setBackslashAsEscape(backslashAsEscapeCheckBox->isChecked());

    if(set && refreshData){
        previewData();
    }
}

void DataImporterCsvOptionsPage::setNullIf(bool refreshData)
{
    bool set = importer.setNullIf(nullIfEditor->text().split(',', QString::SkipEmptyParts));

    if(set && refreshData){
        previewData();
    }
}

void DataImporterCsvOptionsPage::createForm(QVBoxLayout *mainLayout)
{
    QHBoxLayout *hBox = new QHBoxLayout();

    QFormLayout *form1 = new QFormLayout();

    fileEncodingComboBox = new QComboBox();
    WidgetHelper::fillAvailableTextCodecNames(fileEncodingComboBox);
    form1->addRow(tr("Encoding"), fileEncodingComboBox);

    delimiterComboBox = new QComboBox();
    WidgetHelper::fillFieldDelimiters(delimiterComboBox);
    delimiterComboBox->insertItem(1, tr(", and space"), ", ");
    form1->addRow(tr("Field delimiter"), delimiterComboBox);

    skipRowsSpinBox = new QSpinBox();
    skipRowsSpinBox->setRange(0, 100000);
    form1->addRow(tr("Skip rows"), skipRowsSpinBox);

    headerComboBox = new QComboBox();
    headerComboBox->addItem(tr("None"));
    headerComboBox->addItem(tr("Before skip"));
    headerComboBox->addItem(tr("After skip"));
    headerComboBox->setCurrentIndex(2);
    form1->addRow(tr("Header"), headerComboBox);

    QFormLayout *form2 = new QFormLayout();

    enclosureEditor = new QLineEdit();
    enclosureEditor->setToolTip(tr("Comma separated list of enclosing characters."));
    enclosureEditor->setText("\",'");
    form2->addRow(tr("Fields enclosed by"), enclosureEditor);

    nullIfEditor = new QLineEdit();
    nullIfEditor->setToolTip(tr("Comma separated list of strings to be treated as NULL."));
    nullIfEditor->setText(tr("NULL"));
    form2->addRow(tr("Null when"), nullIfEditor);

    backslashAsEscapeCheckBox = new QCheckBox();
    backslashAsEscapeCheckBox->setToolTip(tr("Use backslash (\\) as escape character inside enclosed fields."));
    form2->addRow(tr("Backslash as escape"), backslashAsEscapeCheckBox);

    form1->setContentsMargins(0,0,0,0);
    form2->setContentsMargins(0,0,0,0);
    hBox->addLayout(form1);
    hBox->addLayout(form2);

    hBox->setStretchFactor(form1, 1);
    hBox->setStretchFactor(form2, 1);

    mainLayout->addLayout(hBox);
}

void DataImporterCsvOptionsPage::previewData()
{
    qDebug("loading data");

    importer.resetPosition();

    WidgetHelper::deleteViewModel(previewTable);

    tableModel = new QStandardItemModel(previewTable);

    importer.readRows(this, 50);

    previewTable->setModel(tableModel);
    previewTable->resizeColumnsToFitContents();
}

QStringList DataImporterCsvOptionsPage::createColumnNames(int count) const
{
    QStringList result;

    for(int i=1; i<=count; ++i){
        result.append(QString("COLUMN%1").arg(i));
    }

    return result;
}

void DataImporterCsvOptionsPage::headerAvailable(const QStringList &headerTitles)
{
    tableModel->setHorizontalHeaderLabels(headerTitles);
}

void DataImporterCsvOptionsPage::rowAvailable(const QStringList &values)
{
    QList<QStandardItem*> rowItems;

    if(tableModel->columnCount()==0){
        tableModel->setHorizontalHeaderLabels(createColumnNames(values.size()));
    }

    int columnCount = tableModel->columnCount();
    for(int i=0; i<qMin(values.size(), columnCount); ++i){
        rowItems.append(new QStandardItem(values.at(i)));
    }

    tableModel->appendRow(rowItems);
}

QStandardItemModel *DataImporterCsvOptionsPage::getDataPreviewModel() const
{
    return tableModel;
}

CsvImporter *DataImporterCsvOptionsPage::getImporter()
{
    return &this->importer;
}

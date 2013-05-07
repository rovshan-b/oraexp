#include "dataimportercsvoptionspage.h"
#include "widgets/datatable.h"
#include "util/widgethelper.h"
#include <QDebug>
#include <QtGui>

DataImporterCsvOptionsPage::DataImporterCsvOptionsPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Format options"));
    setSubTitle(tr("Make sure data displayed in grid is in correct format"));

    QVBoxLayout *mainLayout = new QVBoxLayout();

    createForm(mainLayout);

    previewTable = new DataTable();
    previewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(previewTable);

    setLayout(mainLayout);
}

void DataImporterCsvOptionsPage::initializePage()
{
    setEncoding();
    setDelimiter();
    setEnclosure();
    importer.setFilename(field("fileName").toString());

    previewData();
}

void DataImporterCsvOptionsPage::setEncoding()
{
    importer.setEncoding(fileEncodingComboBox->currentText());
}

void DataImporterCsvOptionsPage::setDelimiter()
{
    importer.setDelimiter(WidgetHelper::getComboBoxUserDataOrText(delimiterComboBox));
}

void DataImporterCsvOptionsPage::setEnclosure()
{
    importer.setEnclosures(enclosureEditor->text().trimmed().split(',', QString::SkipEmptyParts));
}

void DataImporterCsvOptionsPage::createForm(QVBoxLayout *mainLayout)
{
    QHBoxLayout *hBox = new QHBoxLayout();

    QFormLayout *form1 = new QFormLayout();

    fileEncodingComboBox = new QComboBox();
    WidgetHelper::fillAvailableTextCodecNames(fileEncodingComboBox, tr("System"));
    form1->addRow(tr("Encoding"), fileEncodingComboBox);

    delimiterComboBox = new QComboBox();
    WidgetHelper::fillFieldDelimiters(delimiterComboBox);
    form1->addRow(tr("Field delimiter"), delimiterComboBox);

    skipRowsSpinBox = new QSpinBox();
    skipRowsSpinBox->setRange(0, 100000);
    form1->addRow(tr("Skip rows"), skipRowsSpinBox);

    QFormLayout *form3 = new QFormLayout();

    headerComboBox = new QComboBox();
    headerComboBox->addItem(tr("None"));
    headerComboBox->addItem(tr("Before skip"));
    headerComboBox->addItem(tr("After skip"));
    headerComboBox->setCurrentIndex(2);
    form3->addRow(tr("Header"), headerComboBox);

    enclosureEditor = new QLineEdit();
    enclosureEditor->setToolTip(tr("Comma separated list of enclosing characters. For example: \", '"));
    enclosureEditor->setText("\",'");
    form3->addRow(tr("Fields enclosed by"), enclosureEditor);

    form1->setContentsMargins(0,0,0,0);
    form3->setContentsMargins(0,0,0,0);
    hBox->addLayout(form1);
    hBox->addLayout(form3);

    hBox->setStretchFactor(form1, 1);
    hBox->setStretchFactor(form3, 1);

    mainLayout->addLayout(hBox);
}

void DataImporterCsvOptionsPage::previewData()
{
    qDebug("loading data");

    importer.resetPosition();

    WidgetHelper::deleteViewModel(previewTable);

    QStandardItemModel *model = new QStandardItemModel(previewTable);

    int readCount = 0;
    while(!importer.isEOF() && (readCount++) < 50){
        QStringList values = importer.readValues();
        if(!values.isEmpty()){
            qDebug() << values;

            if(model->columnCount()==0){
                model->setColumnCount(values.size());
            }

            QList<QStandardItem*> rowItems;

            for(int i=0; i<qMin(values.size(), model->columnCount()); ++i){
                rowItems.append(new QStandardItem(values.at(i)));
            }

            model->appendRow(rowItems);
        }
    }

    previewTable->setModel(model);
}

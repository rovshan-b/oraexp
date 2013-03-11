#include "dataexportoptionswidget.h"
#include "widgets/lineeditwithbutton.h"
#include "widgets/datatableandtoolbarwidget.h"
#include "models/genericeditabletablemodel.h"
#include "delegates/autoappenddelegate.h"
#include "util/widgethelper.h"
#include "util/dialoghelper.h"
#include "util/strutil.h"
#include "exporters/csvexporter.h"
#include "exporters/excelexporter.h"
#include "exporters/htmlexporter.h"
#include "exporters/xmlexporter.h"
#include <QtGui>

DataExportOptionsWidget::DataExportOptionsWidget(QWidget *parent) :
    QWidget(parent),
    selectionStartRow(-1),
    selectionStartColumn(-1),
    selectionEndRow(-1),
    selectionEndColumn(-1)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QFormLayout *form = new QFormLayout();

    formatComboBox = new QComboBox();
    form->addRow(tr("Export format"), formatComboBox);

    filenameEditor = new LineEditWithButton(this);
    form->addRow(tr("File name"), filenameEditor);

    encodingComboBox = new QComboBox();
    WidgetHelper::fillAvailableTextCodecNames(encodingComboBox);
    form->addRow(tr("Encoding"), encodingComboBox);

    bomCheckbox = new QCheckBox(tr("BOM"));
    form->addRow(bomCheckbox);

    mainLayout->addLayout(form);

    tab = new QTabWidget();
    createOptionsTab();
    createDataReplacementTab();
    mainLayout->addWidget(tab);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    populateExportFormats();

    connect(formatComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(correctFileSuffix()));
    connect(filenameEditor, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(selectSaveFilename()));
    connect(includeColumnHeadersCheckbox, SIGNAL(stateChanged(int)), this, SLOT(enableControls()));

    enableControls();
}

void DataExportOptionsWidget::setSelectedRange(int startRow, int startColumn, int endRow, int endColumn)
{
    this->selectionStartRow=startRow;
    this->selectionStartColumn=startColumn;
    this->selectionEndRow=endRow;
    this->selectionEndColumn=endColumn;

    bool canExportSelection = (startRow!=-1 && startColumn!=-1 && endRow!=-1 && endColumn!=-1);
    selectedOnlyCheckbox->setEnabled(canExportSelection);
}

void DataExportOptionsWidget::selectSaveFilename()
{
    QString filename=DialogHelper::showFileSaveDialog(this, formatComboBox->itemData(formatComboBox->currentIndex()).toString());
    if(!filename.isEmpty()){
        filenameEditor->lineEdit()->setText(filename);
    }
}

void DataExportOptionsWidget::enableControls()
{
    quoteColumnHeadersCheckbox->setEnabled(includeColumnHeadersCheckbox->isChecked() &&
                                           includeColumnHeadersCheckbox->isEnabled());
}

void DataExportOptionsWidget::correctFileSuffix()
{
    QString filename=filenameEditor->lineEdit()->text().trimmed();

    if(filename.isEmpty()){
        return;
    }

    QFileInfo fileInfo(filename);

    QString currentSuffix = fileInfo.suffix();
    if(currentSuffix.isEmpty()){
        return;
    }
    QString newSuffix=formatComboBox->itemData(formatComboBox->currentIndex()).toString();
    Q_ASSERT(newSuffix.size());

    if(currentSuffix.compare(newSuffix)==0){
        return;
    }

    filename.chop(currentSuffix.size());
    filename.append(newSuffix);

    filenameEditor->lineEdit()->setText(filename);
}

void DataExportOptionsWidget::createOptionsTab()
{
    QVBoxLayout *layout = new QVBoxLayout();

    QHBoxLayout *vboxContainerLayout = new QHBoxLayout();
    QVBoxLayout *firstColLayout = new QVBoxLayout();
    QVBoxLayout *secondColLayout = new QVBoxLayout();
    vboxContainerLayout->addLayout(firstColLayout);
    vboxContainerLayout->addLayout(secondColLayout);
    vboxContainerLayout->setAlignment(firstColLayout, Qt::AlignTop);
    vboxContainerLayout->setAlignment(secondColLayout, Qt::AlignTop);

    createGeneralOptionsPane(firstColLayout);
    createQuotingOptionsPane(firstColLayout);
    createDelimiterOptionsPane(secondColLayout);

    layout->addLayout(vboxContainerLayout);

    QWidget *optionsTabWidget = new QWidget();
    optionsTabWidget->setLayout(layout);

    tab->addTab(optionsTabWidget, tr("Options"));
}

void DataExportOptionsWidget::createDataReplacementTab()
{
    QVBoxLayout *layout = new QVBoxLayout();

    QLabel *label=new QLabel(tr("In this table you can enter list of strings/characters to be replaced by some other strings/characters."
                                " Enter one replacement rule per row. You can use \\n for newlines, \\r for carriage return and \\t for tab symbols."));
    label->setWordWrap(true);

    layout->addWidget(label);

    GenericEditableTableModel *model = new GenericEditableTableModel(QStringList() << tr("From") << tr("To"), this);
    dataTableAndToolbar = new DataTableAndToolBarWidget(model);
    dataTableAndToolbar->table()->setEditTriggers(QAbstractItemView::AllEditTriggers);
    dataTableAndToolbar->table()->setItemDelegateForColumn(0, new AutoAppendDelegate(this));
    layout->addWidget(dataTableAndToolbar);

    QWidget *dataReplacementTabWidget = new QWidget();
    dataReplacementTabWidget->setLayout(layout);
    tab->addTab(dataReplacementTabWidget, tr("Data replacement"));
}

void DataExportOptionsWidget::createGeneralOptionsPane(QBoxLayout *layout)
{
    QGridLayout *grid = new QGridLayout();

    selectedOnlyCheckbox = new QCheckBox(tr("Export only selected rows"));
    selectedOnlyCheckbox->setEnabled(false);
    grid->addWidget(selectedOnlyCheckbox, 0, 0);

    includeNullTextCheckbox = new QCheckBox(tr("Include null text"));
    grid->addWidget(includeNullTextCheckbox, 1, 0);

    includeColumnHeadersCheckbox = new QCheckBox(tr("Include column headers"));
    includeColumnHeadersCheckbox->setChecked(true);
    grid->addWidget(includeColumnHeadersCheckbox, 2, 0);

    quoteColumnHeadersCheckbox = new QCheckBox(tr("Quote column headers"));
    grid->addWidget(quoteColumnHeadersCheckbox, 3, 0);

    QGroupBox *box = new QGroupBox(tr("General options"));
    box->setLayout(grid);

    layout->addWidget(box);
}

void DataExportOptionsWidget::createQuotingOptionsPane(QBoxLayout *layout)
{
    QFormLayout *form = new QFormLayout();
    stringQuotingComboBox = new QComboBox();
    populateQuotingOptions(stringQuotingComboBox);
    form->addRow(tr("String quoting"), stringQuotingComboBox);

    numberQuotingComboBox = new QComboBox();
    populateQuotingOptions(numberQuotingComboBox);
    form->addRow(tr("Number quoting"), numberQuotingComboBox);

    QGroupBox *box = new QGroupBox(tr("Quoting options"));
    box->setLayout(form);

    layout->addWidget(box);
}

void DataExportOptionsWidget::createDelimiterOptionsPane(QBoxLayout *layout)
{
    QFormLayout *form = new QFormLayout();

    lineEndingsComboBox = new QComboBox();
    populateLineEndingOptions(lineEndingsComboBox);
    form->addRow(tr("Line endings"), lineEndingsComboBox);

    delimiterComboBox = new QComboBox();
    populateDelimiterOptions(delimiterComboBox);
    form->addRow(tr("Delimiter"), delimiterComboBox);

    delimiterAfterLastColumnCheckbox = new QCheckBox(tr("Delimiter after last column"));
    form->addRow(delimiterAfterLastColumnCheckbox);

    QGroupBox *box = new QGroupBox(tr("Delimiters"));
    box->setLayout(form);

    layout->addWidget(box);
}

void DataExportOptionsWidget::populateExportFormats()
{
    formatComboBox->addItem(tr("CSV"), "csv");
    formatComboBox->addItem(tr("Excel"), "xlsx");
    formatComboBox->addItem(tr("HTML"), "html");
    formatComboBox->addItem(tr("XML"), "xml");
    //formatWidgetsTab->addWidget(new CsvExportOptionsWidget());
}


void DataExportOptionsWidget::populateQuotingOptions(QComboBox *comboBox)
{
    comboBox->addItem(tr("None"), "");
    comboBox->addItem(tr("Single quoted"), "'");
    comboBox->addItem(tr("Double quoted"), "\"");
}

void DataExportOptionsWidget::populateLineEndingOptions(QComboBox *comboBox)
{
#ifdef Q_WS_WIN
    QString platformLineEnding = "\r\n";
#else
    QString platformLineEnding = "\n";
#endif

    comboBox->addItem(tr("Platform default"), platformLineEnding);
    comboBox->addItem(tr("Unix style"), "\n");
    comboBox->addItem(tr("Windows style"), "\r\n");
}

void DataExportOptionsWidget::populateDelimiterOptions(QComboBox *comboBox)
{
    comboBox->setEditable(true);
    comboBox->addItem(tr("Comma"), ",");
    comboBox->addItem(tr("Tab"), "\t");
}

void DataExportOptionsWidget::populateNewlineReplacements(QComboBox *comboBox)
{
    comboBox->setEditable(true);
    comboBox->addItem("");
    comboBox->addItem(tr("Blank"), "");
    comboBox->addItem(tr("Tab"), "\t");
    comboBox->addItem(tr("\\n"), "\\n");

    comboBox->setCurrentIndex(0);
}

DataExporterBase *DataExportOptionsWidget::createExporter() const
{
    DataExporterBase *exporter;

    ExportFormat format = (ExportFormat)formatComboBox->currentIndex();
    switch(format){
    case CSV:
        exporter=new CsvExporter();
        break;
    case Excel:
        exporter=new ExcelExporter();
        break;
    case HTML:
        exporter=new HtmlExporter();
        break;
    case XML:
        exporter=new XmlExporter();
        break;
    default:
        exporter=0;
        Q_ASSERT(false);
        break;
    }

    exporter->filename = filenameEditor->lineEdit()->text().trimmed();
    exporter->encoding = encodingComboBox->currentText();
    exporter->bom = bomCheckbox->isChecked();

    bool onlySelectedRows = selectedOnlyCheckbox->isChecked();
    exporter->startRow = onlySelectedRows ? this->selectionStartRow : -1;
    exporter->startColumn = onlySelectedRows ? this->selectionStartColumn : -1;
    exporter->endRow = onlySelectedRows ? this->selectionEndRow : -1;
    exporter->endColumn = onlySelectedRows ? this->selectionEndColumn : -1;

    exporter->includeNullText = includeNullTextCheckbox->isChecked();
    exporter->includeColumnHeaders = includeColumnHeadersCheckbox->isChecked();
    exporter->quoteColumnHeaders = quoteColumnHeadersCheckbox->isChecked();
    exporter->stringQuoting = WidgetHelper::getComboBoxUserDataOrText(stringQuotingComboBox);
    exporter->numberQuoting = WidgetHelper::getComboBoxUserDataOrText(numberQuotingComboBox);
    exporter->lineEnding = WidgetHelper::getComboBoxUserDataOrText(lineEndingsComboBox);
    exporter->columnDelimiter = WidgetHelper::getComboBoxUserDataOrText(delimiterComboBox);
    exporter->delimiterAfterLastColumn = delimiterAfterLastColumnCheckbox->isChecked();

    QAbstractItemModel *model = dataTableAndToolbar->table()->model();
    for(int i=0; i<model->rowCount(); ++i){
        QString from = model->index(i, 0).data().toString();
        QString to = model->index(i, 1).data().toString();

        if(!from.isEmpty()){
            replaceGuiEscapeCharactersWithReal(from);
        }
        if(!to.isEmpty()){
            replaceGuiEscapeCharactersWithReal(to);
        }

        if(!from.isEmpty()){
            exporter->stringReplacements.append(new QPair<QString,QString>(from, to));
        }
    }

    return exporter;
}

bool DataExportOptionsWidget::validate()
{
    if(filenameEditor->lineEdit()->text().trimmed().isEmpty()){
        QMessageBox::critical(this, tr("Validation error"), tr("Please, enter the filename."));
        return false;
    }

    return true;
}

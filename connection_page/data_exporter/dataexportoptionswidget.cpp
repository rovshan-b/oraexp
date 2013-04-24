#include "dataexportoptionswidget.h"
#include "widgets/lineeditwithbutton.h"
#include "widgets/datatableandtoolbarwidget.h"
#include "widgets/fileselectorwidget.h"
#include "widgets/fileencodingwidget.h"
#include "models/genericeditabletablemodel.h"
#include "delegates/autoappenddelegate.h"
#include "util/widgethelper.h"
#include "util/dialoghelper.h"
#include "util/strutil.h"
#include "exporters/csvexporter.h"
#include "exporters/excelexporter.h"
#include "exporters/htmlexporter.h"
#include "exporters/xmlexporter.h"
#include "exporters/insertexporter.h"
#include "defines.h"
#include <QtGui>

DataExportOptionsWidget::DataExportOptionsWidget(bool selfContained, QWidget *parent) :
    QWidget(parent),
    selectionStartRow(-1),
    selectionStartColumn(-1),
    selectionEndRow(-1),
    selectionEndColumn(-1),
    multiTableExport(false)
{
    if(selfContained){
        createUi();
    }
}

void DataExportOptionsWidget::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QFormLayout *form = new QFormLayout();
    createFileOptionsControls(form);
    mainLayout->addLayout(form);

    tab = new QTabWidget();
    tab->addTab(createOptionsTab(), tr("Options"));
    tab->addTab(createDataReplacementTab(), tr("Data replacement"));
    mainLayout->addWidget(tab);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    connectSignalsAndSlots();
}

void DataExportOptionsWidget::createSeparatedUi(QFormLayout *fileControlsForm, QTabWidget *optionsTab)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    createFileOptionsControls(fileControlsForm);

    tab = 0;
    mainLayout->addWidget(createOptionsTab());
    optionsTab->addTab(createDataReplacementTab(), tr("Data replacement"));

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    connectSignalsAndSlots();

    formatComboBox->setCurrentIndex(DataExporterBase::Insert);
}

void DataExportOptionsWidget::connectSignalsAndSlots()
{
    populateExportFormats();

    fileFormatChanged();
    connect(formatComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(fileFormatChanged()));
    connect(includeColumnHeadersCheckbox, SIGNAL(stateChanged(int)), this, SLOT(enableControls()));
    connect(includeSchemaCheckBox, SIGNAL(stateChanged(int)), this, SLOT(includeSchemaCheckBoxChanged()));

    enableControls();
}

void DataExportOptionsWidget::setSelectedRange(int startRow, int startColumn, int endRow, int endColumn)
{
    Q_ASSERT(selectedOnlyCheckbox);

    this->selectionStartRow=startRow;
    this->selectionStartColumn=startColumn;
    this->selectionEndRow=endRow;
    this->selectionEndColumn=endColumn;

    bool canExportSelection = (startRow!=-1 && startColumn!=-1 && endRow!=-1 && endColumn!=-1);
    selectedOnlyCheckbox->setEnabled(canExportSelection);
}

void DataExportOptionsWidget::setTableName(const QString &schemaName, const QString &tableName)
{
    this->schemaNameEditor->setText(schemaName);
    this->tableNameEditor->setText(tableName);
}

void DataExportOptionsWidget::enableControls()
{
    quoteColumnHeadersCheckbox->setEnabled(includeColumnHeadersCheckbox->isChecked() &&
                                           includeColumnHeadersCheckbox->isEnabledTo(this));
}

void DataExportOptionsWidget::fileFormatChanged()
{
    QString formatSuffix = formatComboBox->itemData(formatComboBox->currentIndex()).toString();
    filenameEditor->setDefaultSuffix(formatSuffix);

    DataExporterBase::ExportFormat format = (DataExporterBase::ExportFormat)formatComboBox->currentIndex();

    includeNullTextCheckbox->setEnabled(format!=DataExporterBase::Insert);
    includeColumnHeadersCheckbox->setEnabled(format==DataExporterBase::CSV || format==DataExporterBase::Excel || format==DataExporterBase::HTML);
    quoteColumnHeadersCheckbox->setEnabled(includeColumnHeadersCheckbox->isEnabledTo(this) && includeColumnHeadersCheckbox->isChecked());
    quotingOptionsBox->setEnabled(format==DataExporterBase::CSV);
    delimiterOptionsBox->setEnabled(format==DataExporterBase::CSV);
    tableNameOptionsBox->setEnabled(format==DataExporterBase::Insert);

    emit exportFormatChanged(format);
}

void DataExportOptionsWidget::includeSchemaCheckBoxChanged()
{
    schemaNameEditor->setEnabled(includeSchemaCheckBox->isChecked());
}

void DataExportOptionsWidget::createFileOptionsControls(QFormLayout *form)
{
    formatComboBox = new QComboBox();
    form->addRow(tr("Export format"), formatComboBox);

    filenameEditor = new FileSelectorWidget(this);
    form->addRow(tr("File name"), filenameEditor);

    encodingEditor = new FileEncodingWidget();
    form->addRow(tr("Encoding"), encodingEditor);
}

QWidget *DataExportOptionsWidget::createOptionsTab()
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);

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
    createTargetTableOptionsPane(secondColLayout);

    layout->addLayout(vboxContainerLayout);

    QWidget *optionsTabWidget = new QWidget();
    optionsTabWidget->setLayout(layout);

    return optionsTabWidget;
}

QWidget *DataExportOptionsWidget::createDataReplacementTab()
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

    return dataReplacementTabWidget;
}

void DataExportOptionsWidget::createGeneralOptionsPane(QBoxLayout *layout)
{
    QGridLayout *grid = new QGridLayout();

    if(!multiTableExport){
        selectedOnlyCheckbox = new QCheckBox(tr("Export only selected rows"));
        selectedOnlyCheckbox->setEnabled(false);
        grid->addWidget(selectedOnlyCheckbox, 0, 0);
    }

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

    quotingOptionsBox = new QGroupBox(tr("Quoting options"));
    quotingOptionsBox->setLayout(form);

    layout->addWidget(quotingOptionsBox);
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

    delimiterOptionsBox = new QGroupBox(tr("Delimiters"));
    delimiterOptionsBox->setLayout(form);

    layout->addWidget(delimiterOptionsBox);
}

void DataExportOptionsWidget::createTargetTableOptionsPane(QBoxLayout *layout)
{
    QFormLayout *form = new QFormLayout();

    includeSchemaCheckBox = new QCheckBox();
    includeSchemaCheckBox->setChecked(true);
    form->addRow(tr("Include schema"), includeSchemaCheckBox);

    schemaNameEditor = new QLineEdit();
    schemaNameEditor->setMaxLength(MAX_IDENTIFIER_LENGTH);
    form->addRow(tr("Schema"), schemaNameEditor);

    if(multiTableExport){
        schemaNameEditor->setPlaceholderText(tr("Default"));
    }

    if(!multiTableExport){
        tableNameEditor = new QLineEdit();
        tableNameEditor->setMaxLength(MAX_IDENTIFIER_LENGTH);
        form->addRow(tr("Table"), tableNameEditor);
    }

    tableNameOptionsBox = new QGroupBox(multiTableExport ? tr("Tables") : tr("Table"));
    tableNameOptionsBox->setLayout(form);

    layout->addWidget(tableNameOptionsBox);
}

void DataExportOptionsWidget::populateExportFormats()
{
    formatComboBox->addItem(tr("CSV"), "csv");
    formatComboBox->addItem(tr("Excel"), "xlsx");
    formatComboBox->addItem(tr("HTML"), "html");
    formatComboBox->addItem(tr("XML"), "xml");
    formatComboBox->addItem(tr("Insert"), "sql");
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

DataExporterBase *DataExportOptionsWidget::createExporter()
{
    DataExporterBase *exporter;

    DataExporterBase::ExportFormat format = (DataExporterBase::ExportFormat)formatComboBox->currentIndex();
    switch(format){
    case DataExporterBase::CSV:
        exporter=new CsvExporter();
        break;
    case DataExporterBase::Excel:
        exporter=new ExcelExporter();
        break;
    case DataExporterBase::HTML:
        exporter=new HtmlExporter();
        break;
    case DataExporterBase::XML:
        exporter=new XmlExporter();
        break;
    case DataExporterBase::Insert:
        exporter=new InsertExporter();
        ((InsertExporter*)exporter)->includeSchema = includeSchemaCheckBox->isChecked();
        ((InsertExporter*)exporter)->schemaName=schemaNameEditor->text().trimmed();
        if(!multiTableExport){
            ((InsertExporter*)exporter)->tableName=tableNameEditor->text().trimmed();
        }
        break;
    default:
        exporter=0;
        Q_ASSERT(false);
        break;
    }

    exporter->filename = filenameEditor->fileName();
    exporter->encoding = encodingEditor->encoding();
    exporter->bom = encodingEditor->bom();

    bool onlySelectedRows = multiTableExport ? false : selectedOnlyCheckbox->isChecked();
    exporter->startRow = onlySelectedRows ? this->selectionStartRow : -1;
    exporter->startColumn = onlySelectedRows ? this->selectionStartColumn : -1;
    exporter->endRow = onlySelectedRows ? this->selectionEndRow : -1;
    exporter->endColumn = onlySelectedRows ? this->selectionEndColumn : -1;

    exporter->includeNullText = includeNullTextCheckbox->isChecked();
    exporter->includeColumnHeaders = includeColumnHeadersCheckbox->isEnabledTo(this) && includeColumnHeadersCheckbox->isChecked();
    exporter->quoteColumnHeaders = quoteColumnHeadersCheckbox->isEnabledTo(this) && quoteColumnHeadersCheckbox->isChecked();
    exporter->stringQuoting = stringQuotingComboBox->isEnabledTo(this) ? WidgetHelper::getComboBoxUserDataOrText(stringQuotingComboBox) : "";
    exporter->numberQuoting = numberQuotingComboBox->isEnabledTo(this) ? WidgetHelper::getComboBoxUserDataOrText(numberQuotingComboBox) : "";
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

void DataExportOptionsWidget::setMultiTableMode()
{
    this->multiTableExport=true;
}

bool DataExportOptionsWidget::validate()
{
    if(filenameEditor->fileName().isEmpty()){
        QMessageBox::critical(this, tr("Validation error"), tr("Please, enter the filename."));
        return false;
    }

    return true;
}

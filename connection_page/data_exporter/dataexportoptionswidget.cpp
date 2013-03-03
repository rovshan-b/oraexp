#include "dataexportoptionswidget.h"
#include "widgets/lineeditwithbutton.h"
#include "widgets/datatableandtoolbarwidget.h"
#include "models/genericeditabletablemodel.h"
#include "delegates/autoappenddelegate.h"
#include "util/widgethelper.h"
#include "util/dialoghelper.h"
#include <QtGui>

DataExportOptionsWidget::DataExportOptionsWidget(QWidget *parent) :
    QWidget(parent)
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

    connect(filenameEditor, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(selectSaveFilename()));
}

void DataExportOptionsWidget::selectSaveFilename()
{
    QString filename=DialogHelper::showFileSaveDialog(this, formatComboBox->itemData(formatComboBox->currentIndex()).toString());
    if(!filename.isEmpty()){
        filenameEditor->lineEdit()->setText(filename);
    }
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
    grid->addWidget(selectedOnlyCheckbox, 0, 0);

    includeNullTextCheckbox = new QCheckBox(tr("Include null text"));
    grid->addWidget(includeNullTextCheckbox, 1, 0);

    includeColumnHeadersCheckbox = new QCheckBox(tr("Include column headers"));
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
    formatComboBox->addItem(tr("CSV"));
    formatComboBox->setItemData(0, "csv");
    //formatWidgetsTab->addWidget(new CsvExportOptionsWidget());
}


void DataExportOptionsWidget::populateQuotingOptions(QComboBox *comboBox)
{
    comboBox->addItem(tr("None"));
    comboBox->addItem(tr("Single quoted"));
    comboBox->addItem(tr("Double quoted"));
}

void DataExportOptionsWidget::populateLineEndingOptions(QComboBox *comboBox)
{
    comboBox->addItem(tr("Platform default"));
    comboBox->addItem(tr("Unix style"));
    comboBox->addItem(tr("Windows style"));
}

void DataExportOptionsWidget::populateDelimiterOptions(QComboBox *comboBox)
{
    comboBox->setEditable(true);
    comboBox->addItem(tr("Comma"));
    comboBox->addItem(tr("Tab"));
}

void DataExportOptionsWidget::populateNewlineReplacements(QComboBox *comboBox)
{
    comboBox->setEditable(true);
    comboBox->addItem("");
    comboBox->addItem(tr("Blank"));
    comboBox->addItem(tr("Tab"));
    comboBox->addItem(tr("\\n"));

    comboBox->setCurrentIndex(0);
}

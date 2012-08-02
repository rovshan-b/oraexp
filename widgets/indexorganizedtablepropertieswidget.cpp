#include "indexorganizedtablepropertieswidget.h"
#include "widgets/comboboxandlineeditwidget.h"
#include "widgets/lineeditwithbutton.h"
#include "widgets/lineeditwithautocompleter.h"
#include "interfaces/istringlistretriever.h"
#include "util/strutil.h"
#include "util/widgethelper.h"
#include "util/dialoghelper.h"
#include <QtGui>

IndexOrganizedTablePropertiesWidget::IndexOrganizedTablePropertiesWidget(IQueryScheduler *queryScheduler,
                                                                         IStringListRetriever *columnListRetriever,
                                                                         bool isEditMode,
                                                                         QWidget *parent) :
    QWidget(parent), queryScheduler(queryScheduler), columnListRetriever(columnListRetriever), isEditMode(isEditMode), originallyHadOverflow(false)
{
    QFormLayout *layout=new QFormLayout();

    QIntValidator *pctThresholdValidator=new QIntValidator(1, 50, this);

    pctThresholdEditor=new QLineEdit();
    pctThresholdEditor->setValidator(pctThresholdValidator);
    layout->addRow(tr("% Threshold"), pctThresholdEditor);

    mappingTableComboBox=new QComboBox();
    mappingTableComboBox->addItem("");
    mappingTableComboBox->addItem(tr("Enable"));
    mappingTableComboBox->addItem(tr("Disable"));
    if(isEditMode){
        mappingTableComboBox->setEnabled(false);
    }
    layout->addRow(tr("Mapping table"), mappingTableComboBox);

    keyCompressionEditor=new ComboBoxAndLineEditWidget(true, tr("[Prefix length]"));
    keyCompressionEditor->comboBox()->addItem("");
    keyCompressionEditor->comboBox()->addItem(tr("Enable"));
    keyCompressionEditor->comboBox()->addItem(tr("Disable"));
    if(isEditMode){
        keyCompressionEditor->setEnabled(false);
    }
    layout->addRow(tr("Key compression"), keyCompressionEditor);

    overflowCheckBox=new QCheckBox();
    layout->addRow(tr("Overflow"), overflowCheckBox);

    includingComboBox=new QComboBox();
    includingComboBox->setEditable(true);
    includingComboBox->addItem("");
    includingComboBox->addItems(columnListRetriever->getStringList());
    layout->addRow(tr("Including"), includingComboBox);

    storageEditor=new LineEditWithButton(this);
    layout->addRow(tr("Storage"), storageEditor);

    loggingComboBox=new QComboBox();
    loggingComboBox->addItem("");
    loggingComboBox->addItem(tr("Enable"));
    loggingComboBox->addItem(tr("Disable"));
    layout->addRow(tr("Logging"), loggingComboBox);

    enableOverflowPropertiesWidgets(false);

    setLayout(layout);

    connect(storageEditor, SIGNAL(buttonClicked(LineEditWithButton*)), this, SLOT(storageParamsButtonClicked(LineEditWithButton*)));
    connect(overflowCheckBox, SIGNAL(toggled(bool)), this, SLOT(enableOverflowPropertiesWidgets(bool)));
}

void IndexOrganizedTablePropertiesWidget::storageParamsButtonClicked(LineEditWithButton *lineEditWithButton)
{
    if(DialogHelper::showStorageParamsDialog(this, this->queryScheduler, originallyHadOverflow, true, this->storageParams)){
        lineEditWithButton->lineEdit()->setText(
                    isEditMode
                    ? this->storageParams.generateDiffDdl(originalAttributes.storageParams)
                    : this->storageParams.generateDdl()
                    );
        lineEditWithButton->lineEdit()->setCursorPosition(0);
    }
}

void IndexOrganizedTablePropertiesWidget::enableOverflowPropertiesWidgets(bool enable)
{
    includingComboBox->setEnabled(enable);
    storageEditor->setEnabled(enable);
    loggingComboBox->setEnabled(enable);
}

IndexOrganizedTableProperties IndexOrganizedTablePropertiesWidget::getAttributes() const
{
    IndexOrganizedTableProperties attributes;

    attributes.pctThreshold=getEditorIntValue(pctThresholdEditor->text());
    attributes.mappingTable=mappingTableComboBox->currentIndex();
    attributes.keyCompression=keyCompressionEditor->comboBox()->currentIndex();
    attributes.keyCompressionPrefixLength=getEditorIntValue(keyCompressionEditor->lineEdit()->text());
    attributes.overflow=overflowCheckBox->isChecked();
    attributes.including=includingComboBox->currentText().trimmed();
    attributes.storageParams=this->storageParams;
    attributes.logging=loggingComboBox->currentIndex();

    return attributes;
}

void IndexOrganizedTablePropertiesWidget::setAttributes(const IndexOrganizedTableProperties &attributes,
                                                        const IndexOrganizedTableProperties &originalAttributes)
{
    pctThresholdEditor->setText(getEditorStrValue(attributes.pctThreshold));
    mappingTableComboBox->setCurrentIndex(attributes.mappingTable);
    keyCompressionEditor->comboBox()->setCurrentIndex(attributes.keyCompression);
    keyCompressionEditor->lineEdit()->setText(getEditorStrValue(attributes.keyCompressionPrefixLength));
    overflowCheckBox->setChecked(attributes.overflow);
    WidgetHelper::setComboBoxText(includingComboBox, attributes.including);

    this->storageParams=attributes.storageParams;
    storageEditor->lineEdit()->setText(
                isEditMode
                ? this->storageParams.generateDiffDdl(originalAttributes.storageParams)
                : this->storageParams.generateDdl()
                );
    storageEditor->lineEdit()->setCursorPosition(0);

    loggingComboBox->setCurrentIndex(attributes.logging);


    if(isEditMode && overflowCheckBox->isChecked() && originalAttributes.overflow){
        overflowCheckBox->setEnabled(false);
    }
    originallyHadOverflow=originalAttributes.overflow;

    this->originalAttributes=originalAttributes;
}

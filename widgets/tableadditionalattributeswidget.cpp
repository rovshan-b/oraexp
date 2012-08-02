#include "tableadditionalattributeswidget.h"
#include "widgets/comboboxandlineeditwidget.h"
#include "util/strutil.h"
#include "connectivity/dbconnection.h"
#include <QtGui>

TableAdditionalAttributesWidget::TableAdditionalAttributesWidget(DbConnection *db, OraExp::TableType tableType,
                                                                 bool isEditMode,
                                                                 QWidget *parent) :
    QWidget(parent)
{
    parallelComboBox=new QComboBox();
    parallelDegreeEditor=new QLineEdit();
    loggingComboBox=new QComboBox();
    cacheComboBox=new QComboBox();
    monitoringComboBox=new QComboBox();
    compressComboBox=new QComboBox();
    rowDependenciesComboBox=new QComboBox();
    rowMovementComboBox=new QComboBox();

    parallelComboBox->addItem("");
    parallelComboBox->addItem(tr("Enable"));
    parallelComboBox->addItem(tr("Disable"));

    QIntValidator *validator=new QIntValidator(this);
    validator->setBottom(0);
    parallelDegreeEditor->setValidator(validator);

    loggingComboBox->addItem("");
    loggingComboBox->addItem(tr("Enable"));
    loggingComboBox->addItem(tr("Disable"));

    cacheComboBox->addItem("");
    cacheComboBox->addItem(tr("Enable"));
    cacheComboBox->addItem(tr("Disable"));

    monitoringComboBox->addItem("");
    monitoringComboBox->addItem(tr("Enable"));
    monitoringComboBox->addItem(tr("Disable"));

    compressComboBox->addItem("");
    compressComboBox->addItem(tr("Enable"));
    compressComboBox->addItem(tr("Disable"));
    compressComboBox->addItem(tr("For OLTP (11gR2)"));

    rowDependenciesComboBox->addItem("");
    rowDependenciesComboBox->addItem(tr("Enable"));
    rowDependenciesComboBox->addItem(tr("Disable"));
    if(isEditMode){
        rowDependenciesComboBox->setEnabled(false);
    }

    rowMovementComboBox->addItem("");
    rowMovementComboBox->addItem(tr("Enable"));
    rowMovementComboBox->addItem(tr("Disable"));

    flashbackArchiveEditor=new ComboBoxAndLineEditWidget(false, tr("[Flashback archive]"));
    flashbackArchiveEditor->comboBox()->addItem("");
    flashbackArchiveEditor->comboBox()->addItem(tr("Enable"));
    flashbackArchiveEditor->comboBox()->addItem(tr("Disable"));
    if(!db->supportsFlashbackArchive()){
        flashbackArchiveEditor->setEnabled(false);
        flashbackArchiveEditor->setToolTip(tr("Oracle 11g or later"));
    }

    QFormLayout *form=new QFormLayout();

    form->addRow(tr("Parallel"), parallelComboBox);
    form->addRow(tr("Parallel Degree"), parallelDegreeEditor);
    form->addRow(tr("Logging"), loggingComboBox);
    form->addRow(tr("Cache"), cacheComboBox);
    form->addRow(tr("Monitoring"), monitoringComboBox);
    form->addRow(tr("Compress"), compressComboBox);
    form->addRow(tr("Row Dependencies"), rowDependenciesComboBox);
    form->addRow(tr("Row Movement"), rowMovementComboBox);
    form->addRow(tr("Flashback archive"), flashbackArchiveEditor);

    setLayout(form);

    if(tableType==OraExp::TableTypeIndexOrganized){
        cacheComboBox->setEnabled(false);
        loggingComboBox->setEnabled(false);
        compressComboBox->setEnabled(false);
    }

    enableParallelDegreeEditor();
    connect(parallelComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableParallelDegreeEditor()));
}

void TableAdditionalAttributesWidget::enableParallelDegreeEditor()
{
    parallelDegreeEditor->setEnabled(parallelComboBox->currentIndex()==1);
}

TableAdditionalAttributes TableAdditionalAttributesWidget::getAttributes() const
{
    TableAdditionalAttributes attributes;

    attributes.parallel=parallelComboBox->currentIndex();
    attributes.parallelDegree=getEditorIntValue(parallelDegreeEditor->text());
    attributes.logging=loggingComboBox->currentIndex();
    attributes.cache=cacheComboBox->currentIndex();
    attributes.monitoring=monitoringComboBox->currentIndex();
    attributes.compress=compressComboBox->currentIndex();
    attributes.rowDependencies=rowDependenciesComboBox->currentIndex();
    attributes.rowMovement=rowMovementComboBox->currentIndex();
    attributes.flashbackArchive=flashbackArchiveEditor->comboBox()->currentIndex();
    attributes.flashbackArchiveName=flashbackArchiveEditor->lineEdit()->text().trimmed();

    return attributes;
}

void TableAdditionalAttributesWidget::setAttributes(const TableAdditionalAttributes &attributes) const
{
    parallelComboBox->setCurrentIndex(attributes.parallel);
    parallelDegreeEditor->setText(getEditorStrValue(attributes.parallelDegree));
    loggingComboBox->setCurrentIndex(attributes.logging);
    cacheComboBox->setCurrentIndex(attributes.cache);
    monitoringComboBox->setCurrentIndex(attributes.monitoring);
    compressComboBox->setCurrentIndex(attributes.compress);
    rowDependenciesComboBox->setCurrentIndex(attributes.rowDependencies);
    rowMovementComboBox->setCurrentIndex(attributes.rowMovement);
    flashbackArchiveEditor->comboBox()->setCurrentIndex(attributes.flashbackArchive);
    flashbackArchiveEditor->lineEdit()->setText(attributes.flashbackArchiveName);
}

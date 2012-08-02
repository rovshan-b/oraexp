#include "tableddloptionswidget.h"
#include <QtGui>

TableDdlOptionsWidget::TableDdlOptionsWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *mainLayout=new QHBoxLayout();

    QFormLayout *leftLayout=new QFormLayout();
    QFormLayout *rightLayout=new QFormLayout();

    prettyPrintCombo=new QComboBox();
    addStandardItems(prettyPrintCombo);
    prettyPrintCombo->setToolTip(tr("Format the output with indentation and line feeds"));
    leftLayout->addRow(tr("Pretty print"), prettyPrintCombo);

    sqlTerminatorsCombo=new QComboBox();
    addStandardItems(sqlTerminatorsCombo);
    sqlTerminatorsCombo->setToolTip(tr("Append a SQL terminator (; or /) to each DDL statement"));
    leftLayout->addRow(tr("SQL terminator"), sqlTerminatorsCombo);

    segmentAttributesCombo=new QComboBox();
    addStandardItems(segmentAttributesCombo);
    segmentAttributesCombo->setToolTip(tr("Emit segment attributes (physical attributes, storage attributes, tablespace, logging)"));
    leftLayout->addRow(tr("Segmet attributes"), segmentAttributesCombo);
    connect(segmentAttributesCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(enableControls()));

    storageCombo=new QComboBox();
    addStandardItems(storageCombo);
    storageCombo->setToolTip(tr("Emit storage clause"));
    leftLayout->addRow(tr("Storage"), storageCombo);

    tablespaceCombo=new QComboBox();
    addStandardItems(tablespaceCombo);
    tablespaceCombo->setToolTip(tr("Emit tablespace"));
    leftLayout->addRow(tr("Tablespace"), tablespaceCombo);

    constraintsCombo=new QComboBox();
    addStandardItems(constraintsCombo);
    constraintsCombo->setToolTip(tr("Emit all non-referential table constraints"));
    rightLayout->addRow(tr("Constraints"), constraintsCombo);
    connect(constraintsCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(enableControls()));

    constraintsAsAlterCombo=new QComboBox();
    addStandardItems(constraintsAsAlterCombo);
    constraintsAsAlterCombo->setToolTip(tr("Emit table constraints as separate ALTER TABLE statements"));
    rightLayout->addRow(tr("Constraints as ALTER"), constraintsAsAlterCombo);

    refConstraintsCombo=new QComboBox();
    addStandardItems(refConstraintsCombo);
    refConstraintsCombo->setToolTip(tr("Emit all referential constraints (foreign keys)"));
    rightLayout->addRow(tr("Referential constraints"), refConstraintsCombo);

    oidCombo=new QComboBox();
    addStandardItems(oidCombo);
    oidCombo->setToolTip(tr("Emit the OID clause for object tables"));
    rightLayout->addRow(tr("OID"), oidCombo);

    sizeByteKeywordCombo=new QComboBox();
    addStandardItems(sizeByteKeywordCombo);
    sizeByteKeywordCombo->setToolTip(tr("Emit the BYTE keyword as part of the size specification of CHAR and VARCHAR2 columns that use byte semantics"));
    rightLayout->addRow(tr("Size byte keyword"), sizeByteKeywordCombo);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
}

void TableDdlOptionsWidget::addStandardItems(QComboBox *comboBox)
{
    //comboBox->addItem("");
    comboBox->addItem(tr("Enable"));
    comboBox->addItem(tr("Disable"));
}

void TableDdlOptionsWidget::enableControls()
{
    bool segEnabled=(segmentAttributesCombo->currentIndex()==0);
    storageCombo->setEnabled(segEnabled);
    tablespaceCombo->setEnabled(segEnabled);

    constraintsAsAlterCombo->setEnabled(constraintsCombo->currentIndex()==0);
}

ObjectDdlOptions TableDdlOptionsWidget::getOptions() const
{
    ObjectDdlOptions options;

    options.prettyPrint=getWidgetValue(prettyPrintCombo);
    options.sqlTerminators=getWidgetValue(sqlTerminatorsCombo);
    options.segmentAttributes=getWidgetValue(segmentAttributesCombo);
    options.storage=getWidgetValue(storageCombo);
    options.tablespace=getWidgetValue(tablespaceCombo);
    options.constraints=getWidgetValue(constraintsCombo);
    options.constraintsAsAlter=getWidgetValue(constraintsAsAlterCombo);
    options.refConstraints=getWidgetValue(refConstraintsCombo);
    options.oid=getWidgetValue(oidCombo);
    options.sizeByteKeyword=getWidgetValue(sizeByteKeywordCombo);

    return options;
}

void TableDdlOptionsWidget::setOptions(const ObjectDdlOptions &options)
{
    setWidgetValue(prettyPrintCombo, options.prettyPrint);
    setWidgetValue(sqlTerminatorsCombo, options.sqlTerminators);
    setWidgetValue(segmentAttributesCombo, options.segmentAttributes);
    setWidgetValue(storageCombo, options.storage);
    setWidgetValue(tablespaceCombo, options.tablespace);
    setWidgetValue(constraintsCombo, options.constraints);
    setWidgetValue(constraintsAsAlterCombo, options.constraintsAsAlter);
    setWidgetValue(refConstraintsCombo, options.refConstraints);
    setWidgetValue(oidCombo, options.oid);
    setWidgetValue(sizeByteKeywordCombo, options.sizeByteKeyword);

    enableControls();
}

QString TableDdlOptionsWidget::getWidgetValue(QComboBox *comboBox) const
{
    int ix=comboBox->currentIndex();
    if(ix==0){
        return "y";
    }else{
        return "n";
    }
}

void TableDdlOptionsWidget::setWidgetValue(QComboBox *comboBox, const QString &value)
{
    if(value=="y"){
        comboBox->setCurrentIndex(0);
    }else{
        comboBox->setCurrentIndex(1);
    }
}

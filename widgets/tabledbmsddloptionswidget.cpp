#include "tabledbmsddloptionswidget.h"
#include <QtGui>

TableDbmsDdlOptionsWidget::TableDbmsDdlOptionsWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *mainLayout=new QHBoxLayout();

    QVBoxLayout *leftLayout=new QVBoxLayout();
    QVBoxLayout *rightLayout=new QVBoxLayout();

    prettyPrintCheckBox=new QCheckBox(tr("Pretty print"));
    prettyPrintCheckBox->setChecked(true);
    prettyPrintCheckBox->setToolTip(tr("Format the output with indentation and line feeds"));
    leftLayout->addWidget(prettyPrintCheckBox);

    sqlTerminatorsCheckBox=new QCheckBox(tr("SQL terminator"));
    sqlTerminatorsCheckBox->setChecked(true);
    sqlTerminatorsCheckBox->setToolTip(tr("Append a SQL terminator (; or /) to each DDL statement"));
    leftLayout->addWidget(sqlTerminatorsCheckBox);

    segmentAttributesCheckBox=new QCheckBox(tr("Segmet attributes"));
    segmentAttributesCheckBox->setToolTip(tr("Emit segment attributes (physical attributes, storage attributes, tablespace, logging)"));
    leftLayout->addWidget(segmentAttributesCheckBox);
    connect(segmentAttributesCheckBox, SIGNAL(stateChanged(int)), this, SLOT(enableControls()));

    storageCheckBox=new QCheckBox(tr("Storage"));
    storageCheckBox->setToolTip(tr("Emit storage clause"));
    leftLayout->addWidget(storageCheckBox);

    tablespaceCheckBox=new QCheckBox(tr("Tablespace"));
    tablespaceCheckBox->setToolTip(tr("Emit tablespace"));
    leftLayout->addWidget(tablespaceCheckBox);

    constraintsCheckBox=new QCheckBox(tr("Constraints"));
    constraintsCheckBox->setChecked(true);
    constraintsCheckBox->setToolTip(tr("Emit all non-referential table constraints"));
    rightLayout->addWidget(constraintsCheckBox);
    connect(constraintsCheckBox, SIGNAL(stateChanged(int)), this, SLOT(enableControls()));

    constraintsAsAlterCheckBox=new QCheckBox(tr("Constraints as ALTER"));
    constraintsAsAlterCheckBox->setToolTip(tr("Emit table constraints as separate ALTER TABLE statements"));
    rightLayout->addWidget(constraintsAsAlterCheckBox);

    refConstraintsCheckBox=new QCheckBox(tr("Referential constraints"));
    refConstraintsCheckBox->setChecked(true);
    refConstraintsCheckBox->setToolTip(tr("Emit all referential constraints (foreign keys)"));
    rightLayout->addWidget(refConstraintsCheckBox);

    oidCheckBox=new QCheckBox(tr("OID"));
    oidCheckBox->setToolTip(tr("Emit the OID clause for object tables"));
    rightLayout->addWidget(oidCheckBox);

    sizeByteKeywordCheckBox=new QCheckBox(tr("Size byte keyword"));
    sizeByteKeywordCheckBox->setToolTip(tr("Emit the BYTE keyword as part of the size specification of CHAR and VARCHAR2 columns that use byte semantics"));
    rightLayout->addWidget(sizeByteKeywordCheckBox);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    setLayout(mainLayout);
}

void TableDbmsDdlOptionsWidget::enableControls()
{
    bool segEnabled=(segmentAttributesCheckBox->isChecked());
    storageCheckBox->setEnabled(segEnabled);
    tablespaceCheckBox->setEnabled(segEnabled);

    constraintsAsAlterCheckBox->setEnabled(constraintsCheckBox->isChecked());
}

ObjectDdlOptions TableDbmsDdlOptionsWidget::getOptions() const
{
    ObjectDdlOptions options;

    options.prettyPrint=getWidgetValue(prettyPrintCheckBox);
    options.sqlTerminators=getWidgetValue(sqlTerminatorsCheckBox);
    options.segmentAttributes=getWidgetValue(segmentAttributesCheckBox);
    options.storage=getWidgetValue(storageCheckBox);
    options.tablespace=getWidgetValue(tablespaceCheckBox);
    options.constraints=getWidgetValue(constraintsCheckBox);
    options.constraintsAsAlter=getWidgetValue(constraintsAsAlterCheckBox);
    options.refConstraints=getWidgetValue(refConstraintsCheckBox);
    options.oid=getWidgetValue(oidCheckBox);
    options.sizeByteKeyword=getWidgetValue(sizeByteKeywordCheckBox);

    return options;
}

void TableDbmsDdlOptionsWidget::setOptions(const ObjectDdlOptions &options)
{
    setWidgetValue(prettyPrintCheckBox, options.prettyPrint);
    setWidgetValue(sqlTerminatorsCheckBox, options.sqlTerminators);
    setWidgetValue(segmentAttributesCheckBox, options.segmentAttributes);
    setWidgetValue(storageCheckBox, options.storage);
    setWidgetValue(tablespaceCheckBox, options.tablespace);
    setWidgetValue(constraintsCheckBox, options.constraints);
    setWidgetValue(constraintsAsAlterCheckBox, options.constraintsAsAlter);
    setWidgetValue(refConstraintsCheckBox, options.refConstraints);
    setWidgetValue(oidCheckBox, options.oid);
    setWidgetValue(sizeByteKeywordCheckBox, options.sizeByteKeyword);

    enableControls();
}

QString TableDbmsDdlOptionsWidget::getWidgetValue(QCheckBox *checkBox) const
{
    return checkBox->isChecked() ? "y" : "n";
}

void TableDbmsDdlOptionsWidget::setWidgetValue(QCheckBox *checkBox, const QString &value)
{
    checkBox->setChecked(value=="y");
}

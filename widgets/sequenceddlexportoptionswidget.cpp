#include "sequenceddlexportoptionswidget.h"
#include <QtGui>

SequenceDdlExportOptionsWidget::SequenceDdlExportOptionsWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    setInitialValueCheckBox = new QCheckBox(tr("Set start value"));
    mainLayout->addWidget(setInitialValueCheckBox);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

SequenceCreateDdlOptions SequenceDdlExportOptionsWidget::getOptions() const
{
    SequenceCreateDdlOptions options;

    options.setInitialValue = setInitialValueCheckBox->isChecked();

    return options;
}

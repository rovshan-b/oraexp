#include "sequenceddldiffoptionswidget.h"
#include <QtGui>

SequenceDdlDiffOptionsWidget::SequenceDdlDiffOptionsWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    updateCurrValueCheckBox = new QCheckBox(tr("Update current value"));
    mainLayout->addWidget(updateCurrValueCheckBox);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);
}

SequenceDiffDdlOptions SequenceDdlDiffOptionsWidget::getOptions() const
{
    SequenceDiffDdlOptions options;

    options.updateCurrVal = updateCurrValueCheckBox->isChecked();

    return options;
}

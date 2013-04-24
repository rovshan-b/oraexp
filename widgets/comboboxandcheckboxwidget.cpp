#include "comboboxandcheckboxwidget.h"
#include <QtGui>

ComboBoxAndCheckBoxWidget::ComboBoxAndCheckBoxWidget(const QString &checkBoxTitle, QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout=new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);

    combo=new QComboBox();
    layout->addWidget(combo);

    check=new QCheckBox();
    check->setText(checkBoxTitle);
    layout->addWidget(check);

    layout->setSpacing(2);
    setLayout(layout);

    layout->setStretchFactor(combo, 1);
    layout->setAlignment(check, Qt::AlignRight);

    connect(combo, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));
    connect(check, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
}

QComboBox *ComboBoxAndCheckBoxWidget::comboBox() const
{
    return this->combo;
}

QCheckBox *ComboBoxAndCheckBoxWidget::checkBox() const
{
    return this->check;
}


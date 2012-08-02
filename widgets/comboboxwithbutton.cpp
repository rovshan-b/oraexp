#include "comboboxwithbutton.h"
#include <QtGui>

ComboBoxWithButton::ComboBoxWithButton(const QString &initialValue, const QString &iconName, QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout=new QHBoxLayout();
    combo=new DbItemListComboBox(initialValue, iconName, false, true);
    button=new QPushButton(tr("..."));
    button->setMaximumWidth(button->fontMetrics().width("A")*3);

    layout->addWidget(combo);
    layout->addWidget(button);

    layout->setStretch(0, 1);
    layout->setSpacing(2);
    layout->setContentsMargins(0, 0, 0, 0);

    setLayout(layout);

    connect(button, SIGNAL(clicked()), this, SLOT(browseButtonClicked()));

    setFocusProxy(combo);
    setBackgroundRole(palette().Base);
}

DbItemListComboBox *ComboBoxWithButton::comboBox() const
{
    return combo;
}

QPushButton *ComboBoxWithButton::browseButton() const
{
    return button;
}

QLineEdit *ComboBoxWithButton::lineEdit() const
{
    return combo->lineEdit();
}

void ComboBoxWithButton::browseButtonClicked()
{
    emit buttonClicked(this);
}


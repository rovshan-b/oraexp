#include "lineeditandcheckboxwidget.h"
#include <QtGui>

LineEditAndCheckBoxWidget::LineEditAndCheckBoxWidget(const QString &checkBoxTitle, QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout=new QHBoxLayout();
    layout->setContentsMargins(0,0,0,0);

    editor=new QLineEdit();
    layout->addWidget(editor);

    check=new QCheckBox();
    check->setText(checkBoxTitle);
    layout->addWidget(check);

    layout->setSpacing(2);
    setLayout(layout);

    connect(editor, SIGNAL(editingFinished()), this, SIGNAL(changed()));
    connect(check, SIGNAL(toggled(bool)), this, SIGNAL(changed()));
}

QLineEdit *LineEditAndCheckBoxWidget::lineEdit() const
{
    return this->editor;
}

QCheckBox *LineEditAndCheckBoxWidget::checkBox() const
{
    return this->check;
}

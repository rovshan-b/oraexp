#include "passwordeditor.h"
#include <QLineEdit>
#include <QCheckBox>

PasswordEditor::PasswordEditor(QWidget *parent) :
    LineEditAndCheckBoxWidget(tr("Show"), parent)
{
    lineEdit()->setEchoMode(QLineEdit::Password);

    connect(checkBox(), SIGNAL(stateChanged(int)), this, SLOT(showPassword()));
}

void PasswordEditor::showPassword()
{
    lineEdit()->setEchoMode(checkBox()->isChecked() ? QLineEdit::Normal : QLineEdit::Password);
}

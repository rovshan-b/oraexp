#include "gotolinedialog.h"
#include "util/dialoghelper.h"
#include <QtGui>

GoToLineDialog::GoToLineDialog(int currentLine, int maxLineCount, QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QFormLayout *form=new QFormLayout();

    lineNumberSelector=new QSpinBox();
    lineNumberSelector->setMaximum(1);
    lineNumberSelector->setMaximum(maxLineCount);
    lineNumberSelector->setValue(currentLine);

    form->addRow(tr("Line:"), lineNumberSelector);

    mainLayout->addLayout(form);

    QDialogButtonBox *buttonBox=DialogHelper::createButtonBox(this);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    setWindowTitle(tr("Go to line"));

    lineNumberSelector->setFocus();
}

int GoToLineDialog::getLine() const
{
    return lineNumberSelector->value();
}

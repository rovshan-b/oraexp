#include "selectlinkedtablesdialog.h"
#include "util/dialoghelper.h"
#include <QtGui>

SelectLinkedTablesDialog::SelectLinkedTablesDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Relation type"));

    QVBoxLayout *mainLayout=new QVBoxLayout();

    QGroupBox *groupBox = new QGroupBox(tr("Relation type"));

    QVBoxLayout *groupBoxLayout=new QVBoxLayout();

    parentTablesRadio = new QRadioButton(tr("Parent tables"));
    groupBoxLayout->addWidget(parentTablesRadio);

    childTablesRadio = new QRadioButton(tr("Child tables"));
    groupBoxLayout->addWidget(childTablesRadio);

    bothRadio = new QRadioButton(tr("Both"));
    bothRadio->setChecked(true);
    groupBoxLayout->addWidget(bothRadio);

    groupBox->setLayout(groupBoxLayout);
    mainLayout->addWidget(groupBox);

    QFormLayout *form=new QFormLayout();

    maxLevelEditor = new QSpinBox();
    maxLevelEditor->setRange(0, 10000);
    maxLevelEditor->setToolTip(tr("Maximum number of steps to go. Zero means unlimited."));
    form->addRow(tr("Max level"), maxLevelEditor);

    patternEditor = new QLineEdit();
    patternEditor->setText("%");
    patternEditor->setToolTip(tr("Pattern table names must match."));
    form->addRow(tr("Name like"), patternEditor);
    mainLayout->addLayout(form);

    QDialogButtonBox *buttonBox=DialogHelper::createButtonBox(this);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    groupBox->setFocus();
}

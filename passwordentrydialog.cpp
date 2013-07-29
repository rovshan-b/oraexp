#include "passwordentrydialog.h"
#include "ui_passwordentrydialog.h"
#include <QtGui>

PasswordEntryDialog::PasswordEntryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordEntryDialog)
{
    ui->setupUi(this);

    okButton=ui->buttonBox->button(QDialogButtonBox::Ok);
    okButton->setEnabled(false);

    connect(ui->passwordLineEdit, SIGNAL(textEdited(QString)), this, SLOT(validateForm()));
}

PasswordEntryDialog::~PasswordEntryDialog()
{
    delete ui;
}

QString PasswordEntryDialog::password() const
{
    return ui->passwordLineEdit->text();
}

bool PasswordEntryDialog::savePassword() const
{
    return ui->savePasswordCheckBox->isChecked();
}

bool PasswordEntryDialog::isFormValid()
{
    return ui->passwordLineEdit->text().trimmed().length()>0;
}

void PasswordEntryDialog::validateForm()
{
    okButton->setEnabled(isFormValid());
}

#include "connectioneditordialog.h"
#include "ui_connectioneditordialog.h"

#include <QtGui>
#include "connectivity/dbconnection.h"
#include "connectivity/ociexception.h"
#include "util/iconutil.h"
#include "util/strutil.h"
#include "util/widgethelper.h"

ConnectionEditorDialog::ConnectionEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionEditorDialog)
{
    ui->setupUi(this);

    testConnectionButton=new QPushButton(tr("&Test connection"));
    connect(testConnectionButton, SIGNAL(clicked()), this, SLOT(testConnection()));
    ui->buttonBox->addButton(testConnectionButton, QDialogButtonBox::ActionRole);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));

    populateEnvironmentNames();
    populateTnsNames();

    validateForm();

    connect(ui->nameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(validateForm()));
    connect(ui->tnsCombo, SIGNAL(activated(int)), this, SLOT(validateForm()));
    connect(ui->usernameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(validateForm()));
    connect(ui->passwordLineEdit, SIGNAL(textEdited(QString)), this, SLOT(validateForm()));
}

ConnectionEditorDialog::~ConnectionEditorDialog()
{
    delete ui;
}

void ConnectionEditorDialog::populateEnvironmentNames()
{
    ui->environmentCombo->addItem(IconUtil::getEnvironmentIcon(tr("Test")), tr("Test"));
    ui->environmentCombo->addItem(IconUtil::getEnvironmentIcon(tr("Development")), tr("Development"));
    ui->environmentCombo->addItem(IconUtil::getEnvironmentIcon(tr("Production")), tr("Production"));
}

void ConnectionEditorDialog::populateTnsNames()
{
    ui->tnsCombo->addItem("BSCSTEST.AZERCELL.COM");
}

void ConnectionEditorDialog::testConnection()
{
    DbConnection *db=getDbConnection();

    try{
        db->connect();
        QMessageBox::information(this, tr("Connection succeeded"), tr("Successfully connected to database."));
    }catch(OciException &ex){
        QMessageBox::critical(this, tr("Connection failed"), ex.getErrorMessage());
    }

    delete db;
}

DbConnection *ConnectionEditorDialog::getDbConnection()
{
    return new DbConnection(ui->nameLineEdit->text(), ui->environmentCombo->currentText(), ui->tnsCombo->currentText(),
                    ui->usernameLineEdit->text().trimmed().toUpper(), ui->passwordLineEdit->text().trimmed(),
                    ui->savePasswordCheckBox->isChecked());
}

void ConnectionEditorDialog::setDbConnection(DbConnection *db)
{
    ui->nameLineEdit->setText(db->getTitle());
    ui->environmentCombo->setCurrentIndex(ui->environmentCombo->findText(db->getEnvironment()));
    WidgetHelper::setComboBoxText(ui->tnsCombo, db->getTnsName());
    ui->usernameLineEdit->setText(db->getUsername());
    ui->passwordLineEdit->setText(db->getPassword());
    ui->savePasswordCheckBox->setChecked(db->getSavePassword());

    validateForm();
}

bool ConnectionEditorDialog::isFormValid()
{
    return ui->nameLineEdit->text().trimmed().length()>0 && ui->environmentCombo->currentText().length()>0 &&
            ui->tnsCombo->currentText().length()>0 && ui->usernameLineEdit->text().trimmed().length()>0;
}

void ConnectionEditorDialog::validateForm()
{
    bool isValid=isFormValid();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isValid);
    testConnectionButton->setEnabled(isValid);

    bool passwordExists=ui->passwordLineEdit->text().trimmed().length()>0;
    ui->savePasswordCheckBox->setEnabled(passwordExists);
    if(!passwordExists){
        ui->savePasswordCheckBox->setChecked(false);
    }
}

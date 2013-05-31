#include "usercreatorgeneralinfo.h"
#include "widgets/nameeditor.h"
#include "widgets/passwordeditor.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/tablespacelistcombobox.h"
#include "util/widgethelper.h"
#include "interfaces/iqueryscheduler.h"
#include "connectivity/dbconnection.h"
#include "beans/userinfo.h"
#include <QtGui>

UserCreatorGeneralInfo::UserCreatorGeneralInfo(const QString &objectName,
                                               UserCreatorTabs *userCreator,
                                               DbObjectCreator::CreatorMode creatorMode,
                                               QWidget *parent) :
    UserCreatorTab(userCreator, creatorMode, parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QLayout *form=createForm(objectName);
    mainLayout->addLayout(form);

    setLayout(mainLayout);
}

void UserCreatorGeneralInfo::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    UserCreatorTab::setQueryScheduler(queryScheduler);

    enableEditionsCheckBox->setEnabled(queryScheduler->getDb()->supportsSchemaEditions());

    defaultTablespaceComboBox->setQueryScheduler(this->queryScheduler);
    temporaryTablespaceComboBox->setQueryScheduler(this->queryScheduler);
    profileComboBox->loadItems(this->queryScheduler, "get_profile_list");
}

void UserCreatorGeneralInfo::setUserInfo(UserInfo *userInfo)
{
    UserGeneralInfo *info=&userInfo->generalInfo;

    usernameEditor->setText(info->username);
    identifiedByComboBox->setCurrentIndex(info->identifiedBy);
    passwordEditor->lineEdit()->setText(info->password);
    dnEditor->setText(info->dn);
    WidgetHelper::setComboBoxText(defaultTablespaceComboBox->comboBox(), info->defaultTablespace);
    WidgetHelper::setComboBoxText(temporaryTablespaceComboBox->comboBox(), info->temporaryTablespace);
    WidgetHelper::setComboBoxText(profileComboBox, info->profile);
    passwordExpiredCheckBox->setChecked(info->expirePassword);
    accountLockedCheckBox->setChecked(info->accountLocked);
    enableEditionsCheckBox->setChecked(info->enableEditions);

    if(info->expirePassword){
        passwordExpiredCheckBox->setEnabled(false);
    }

    if(info->enableEditions){
        enableEditionsCheckBox->setEnabled(false);
    }
}

UserGeneralInfo UserCreatorGeneralInfo::getUserGeneralInfo() const
{
    UserGeneralInfo info;

    info.username=usernameEditor->text().trimmed().toUpper();
    info.identifiedBy=(UserGeneralInfo::UserIdentifiedBy)identifiedByComboBox->currentIndex();
    info.password=passwordEditor->lineEdit()->text();
    info.dn=dnEditor->text();
    info.defaultTablespace=defaultTablespaceComboBox->lineEdit()->text().trimmed().toUpper();
    info.temporaryTablespace=temporaryTablespaceComboBox->lineEdit()->text().trimmed().toUpper();
    info.profile=profileComboBox->lineEdit()->text().trimmed().toUpper();
    info.expirePassword=passwordExpiredCheckBox->isChecked();
    info.accountLocked=accountLockedCheckBox->isChecked();
    info.enableEditions=enableEditionsCheckBox->isChecked();

    return info;
}

QString UserCreatorGeneralInfo::getUserName() const
{
    return usernameEditor->text().trimmed().toUpper();
}

QLayout *UserCreatorGeneralInfo::createForm(const QString &objectName)
{
    QFormLayout *form=new QFormLayout();

    usernameEditor = new NameEditor();
    usernameEditor->setText(objectName);
    form->addRow(tr("User name"), usernameEditor);

    identifiedByComboBox = new QComboBox();
    identifiedByComboBox->addItem(tr("Password"));
    identifiedByComboBox->addItem(tr("Externally"));
    identifiedByComboBox->addItem(tr("Globally"));
    identifiedByComboBox->setCurrentIndex(0);
    form->addRow(tr("Identified by"), identifiedByComboBox);

    passwordEditor = new PasswordEditor();
    form->addRow(tr("Password"), passwordEditor);

    dnEditor = new QLineEdit();
    form->addRow(tr("DN/External name"), dnEditor);

    defaultTablespaceComboBox = new TablespaceListComboBox();
    form->addRow(tr("Default tablespace"), defaultTablespaceComboBox);

    temporaryTablespaceComboBox = new TablespaceListComboBox();
    form->addRow(tr("Temporary tablespace"), temporaryTablespaceComboBox);

    profileComboBox = new DbItemListComboBox("", "", true);
    form->addRow(tr("Profile"), profileComboBox);

    passwordExpiredCheckBox = new QCheckBox();
    form->addRow(tr("Password expired"), passwordExpiredCheckBox);

    accountLockedCheckBox = new QCheckBox();
    form->addRow(tr("Account locked"), accountLockedCheckBox);

    enableEditionsCheckBox = new QCheckBox();
    form->addRow(tr("Enable editions"), enableEditionsCheckBox);

    enableControls();

    connect(identifiedByComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(enableControls()));

    connect(usernameEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(identifiedByComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(ddlChanged()));
    connect(passwordEditor, SIGNAL(changed()), this, SIGNAL(ddlChanged()));
    connect(dnEditor, SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(defaultTablespaceComboBox->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(temporaryTablespaceComboBox->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(profileComboBox->lineEdit(), SIGNAL(editingFinished()), this, SIGNAL(ddlChanged()));
    connect(passwordExpiredCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(ddlChanged()));
    connect(accountLockedCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(ddlChanged()));
    connect(enableEditionsCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(ddlChanged()));

    if(getCreatorMode() == DbObjectCreator::EditExisting){
        disableControlsForEditMode();
    }

    return form;
}

void UserCreatorGeneralInfo::disableControlsForEditMode()
{
    usernameEditor->setEnabled(false);
}

void UserCreatorGeneralInfo::enableControls()
{
    UserGeneralInfo::UserIdentifiedBy identifiedBy=(UserGeneralInfo::UserIdentifiedBy)identifiedByComboBox->currentIndex();

    passwordEditor->setEnabled(identifiedBy==UserGeneralInfo::Password);
    dnEditor->setEnabled(identifiedBy!=UserGeneralInfo::Password);
}

void UserCreatorGeneralInfo::alterQuerySucceeded(const QString &taskName)
{
    if(taskName=="alter_user"){
        objectCreator->getOriginalUserInfo()->generalInfo=getUserGeneralInfo();

        if(passwordExpiredCheckBox->isEnabledTo(this) && passwordExpiredCheckBox->isChecked()){
            passwordExpiredCheckBox->setEnabled(false);
        }

        if(enableEditionsCheckBox->isEnabledTo(this) && enableEditionsCheckBox->isChecked()){
            enableEditionsCheckBox->setEnabled(false);
        }
    }
}

#include "usercreatorgeneralinfo.h"
#include "widgets/nameeditor.h"
#include "widgets/passwordeditor.h"
#include "widgets/dbitemlistcombobox.h"
#include "util/widgethelper.h"
#include <QtGui>

UserCreatorGeneralInfo::UserCreatorGeneralInfo(const QString &objectName,
                                               UserCreatorTabs *userCreator,
                                               bool editMode,
                                               QWidget *parent) :
    UserCreatorTab(userCreator, editMode, parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QLayout *form=createForm();
    mainLayout->addLayout(form);

    QWidget *rolesBox=createRolesBox();
    mainLayout->addWidget(rolesBox);
    mainLayout->setAlignment(rolesBox, Qt::AlignLeft|Qt::AlignTop);

    QWidget *privBox=createPrivilegesBox();
    mainLayout->addWidget(privBox);
    mainLayout->setAlignment(privBox, Qt::AlignLeft|Qt::AlignTop);

    mainLayout->addStretch();

    setLayout(mainLayout);
}

QLayout *UserCreatorGeneralInfo::createForm()
{
    QFormLayout *form=new QFormLayout();

    usernameEditor = new NameEditor();
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
    form->addRow(tr("Distinguished name"), dnEditor);

    defaultTablespaceComboBox = new DbItemListComboBox("", "tablespace", true);
    form->addRow(tr("Default tablespace"), defaultTablespaceComboBox);

    temporaryTablespaceComboBox = new DbItemListComboBox("", "tablespace", true);
    form->addRow(tr("Temporary tablespace"), temporaryTablespaceComboBox);

    profileComboBox = new DbItemListComboBox("", "", true);
    form->addRow(tr("Profile"), profileComboBox);

    passwordExpiredCheckBox = new QCheckBox();
    form->addRow(tr("Password expired"), passwordExpiredCheckBox);

    accountLockedCheckBox = new QCheckBox();
    form->addRow(tr("Account locked"), accountLockedCheckBox);

    enableEditionsCheckBox = new QCheckBox();
    form->addRow(tr("Enable editions"), enableEditionsCheckBox);

    return form;
}

QWidget *UserCreatorGeneralInfo::createRolesBox()
{
    roleList << "CONNECT" << "RESOURCE" << "DBA";

    QGroupBox *rolesBox = new QGroupBox(tr("Roles"));
    rolesBox->setLayout(WidgetHelper::createCheckBoxes(roleList));

    return rolesBox;
}

QWidget *UserCreatorGeneralInfo::createPrivilegesBox()
{
    privList << "CREATE DATABASE LINK" << "CREATE MATERIALIZED VIEW"
             << "CREATE PROCEDURE" << "CREATE PUBLIC SYNONYM"
             << "CREATE ROLE" << "CREATE SEQUENCE" << "CREATE SYNONYM"
             << "CREATE TABLE" << "CREATE TRIGGER" << "CREATE TYPE"
             << "CREATE VIEW";

    QGroupBox *privBox = new QGroupBox(tr("Privileges"));
    privBox->setLayout(WidgetHelper::createCheckBoxes(privList));

    return privBox;
}

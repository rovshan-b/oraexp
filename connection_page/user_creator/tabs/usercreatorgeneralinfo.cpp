#include "usercreatorgeneralinfo.h"
#include "widgets/nameeditor.h"
#include "widgets/passwordeditor.h"
#include "widgets/dbitemlistcombobox.h"
#include "widgets/tablespacelistcombobox.h"
#include "util/widgethelper.h"
#include <QtGui>

UserCreatorGeneralInfo::UserCreatorGeneralInfo(const QString &objectName,
                                               UserCreatorTabs *userCreator,
                                               bool editMode,
                                               QWidget *parent) :
    UserCreatorTab(userCreator, editMode, parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QLayout *form=createForm(objectName);
    mainLayout->addLayout(form);

    setLayout(mainLayout);
}

void UserCreatorGeneralInfo::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    UserCreatorTab::setQueryScheduler(queryScheduler);

    defaultTablespaceComboBox->setQueryScheduler(this->queryScheduler);
    temporaryTablespaceComboBox->setQueryScheduler(this->queryScheduler);
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
    form->addRow(tr("Distinguished name"), dnEditor);

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

    return form;
}

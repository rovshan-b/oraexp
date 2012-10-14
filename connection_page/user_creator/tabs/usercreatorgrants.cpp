#include "usercreatorgrants.h"
#include "util/widgethelper.h"
#include <QtGui>

UserCreatorGrants::UserCreatorGrants(UserCreatorTabs *userCreator, bool editMode, QWidget *parent) :
    UserCreatorTab(userCreator, editMode, parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QHBoxLayout *modeSelectionLayout=new QHBoxLayout();
    simpleModeRadio=new QRadioButton(tr("&Simple mode"));
    simpleModeRadio->setChecked(true);
    modeSelectionLayout->addWidget(simpleModeRadio);
    advancedModeRadio=new QRadioButton(tr("&Advanced mode"));
    modeSelectionLayout->addWidget(advancedModeRadio);

    mainLayout->addLayout(modeSelectionLayout);
    mainLayout->setAlignment(modeSelectionLayout, Qt::AlignTop|Qt::AlignLeft);

    tab = new QStackedWidget();

    createSimpleLayout();
    createAdvancedLayout();

    mainLayout->addWidget(tab);
    setLayout(mainLayout);

    connect(simpleModeRadio, SIGNAL(toggled(bool)), this, SLOT(switchMode(bool)));
}

void UserCreatorGrants::checkAllQuickRoles()
{
    WidgetHelper::setCheckedBoxes(rolesBox, QStringList(), true, true);
}

void UserCreatorGrants::uncheckAllQuickRoles()
{
    WidgetHelper::setCheckedBoxes(rolesBox, QStringList(), false, true);
}

void UserCreatorGrants::checkAllQuickPrivs()
{
    WidgetHelper::setCheckedBoxes(privBox, QStringList(), true, true);
}

void UserCreatorGrants::uncheckAllQuickPrivs()
{
    WidgetHelper::setCheckedBoxes(privBox, QStringList(), false, true);
}

void UserCreatorGrants::switchMode(bool simpleMode)
{
    tab->setCurrentIndex(simpleMode ? 0 : 1);
}

void UserCreatorGrants::createSimpleLayout()
{
    QWidget *simpleLayoutWidget=new QWidget();
    QVBoxLayout *simpleLayout=new QVBoxLayout();

    QWidget *rolesBox=createRolesBox();
    simpleLayout->addWidget(rolesBox);
    simpleLayout->setAlignment(rolesBox, Qt::AlignLeft|Qt::AlignTop);

    QWidget *privBox=createPrivilegesBox();
    simpleLayout->addWidget(privBox);
    simpleLayout->setAlignment(privBox, Qt::AlignLeft|Qt::AlignTop);

    simpleLayout->addStretch();

    simpleLayoutWidget->setLayout(simpleLayout);

    tab->addWidget(simpleLayoutWidget);
}

void UserCreatorGrants::createAdvancedLayout()
{
    tab->addWidget(new QWidget());
}

QWidget *UserCreatorGrants::createRolesBox()
{
    quickRoleList << "CONNECT" << "RESOURCE" << "DBA";

    rolesBox = new QGroupBox(tr("Roles"));
    QGridLayout *grid=WidgetHelper::createCheckBoxes(quickRoleList, 3, QStringList() << "CONNECT" << "RESOURCE");
    WidgetHelper::createCheckUncheckButtons(grid, this, SLOT(checkAllQuickRoles()), SLOT(uncheckAllQuickRoles()));
    rolesBox->setLayout(grid);

    return rolesBox;
}

QWidget *UserCreatorGrants::createPrivilegesBox()
{
    quickPrivList << "CREATE DATABASE LINK" << "CREATE MATERIALIZED VIEW"
             << "CREATE PROCEDURE" << "CREATE PUBLIC SYNONYM"
             << "CREATE ROLE" << "CREATE SEQUENCE" << "CREATE SYNONYM"
             << "CREATE TABLE" << "CREATE TRIGGER" << "CREATE TYPE"
             << "CREATE VIEW";

    privBox = new QGroupBox(tr("Privileges"));
    QGridLayout *grid=WidgetHelper::createCheckBoxes(quickPrivList, 3);
    WidgetHelper::createCheckUncheckButtons(grid, this, SLOT(checkAllQuickPrivs()), SLOT(uncheckAllQuickPrivs()));
    privBox->setLayout(grid);

    return privBox;
}

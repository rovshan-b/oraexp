#include "usercreatorgrantssimplelayout.h"
#include "util/widgethelper.h"
#include <QtGui>

UserCreatorGrantsSimpleLayout::UserCreatorGrantsSimpleLayout(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    QWidget *rolesBox=createRolesBox();
    mainLayout->addWidget(rolesBox);
    mainLayout->setAlignment(rolesBox, Qt::AlignLeft|Qt::AlignTop);

    QWidget *privBox=createPrivilegesBox();
    mainLayout->addWidget(privBox);
    mainLayout->setAlignment(privBox, Qt::AlignLeft|Qt::AlignTop);

    mainLayout->addStretch();

    mainLayout->setContentsMargins(0,0,0,0);

    setLayout(mainLayout);
}

QList<QCheckBox *> UserCreatorGrantsSimpleLayout::getRoleCheckBoxes() const
{
    return rolesBox->findChildren<QCheckBox*>();
}

QList<QCheckBox *> UserCreatorGrantsSimpleLayout::getPrivCheckBoxes() const
{
    return privBox->findChildren<QCheckBox*>();
}

QWidget *UserCreatorGrantsSimpleLayout::createRolesBox()
{
    quickRoleList << "CONNECT" << "RESOURCE" << "DBA";

    rolesBox = new QGroupBox(tr("Roles"));
    QGridLayout *grid=WidgetHelper::createCheckBoxes(quickRoleList, 3, QStringList() << "CONNECT" << "RESOURCE");
    WidgetHelper::createCheckUncheckButtons(grid, this, SLOT(checkAllQuickRoles()), SLOT(uncheckAllQuickRoles()));
    rolesBox->setLayout(grid);

    return rolesBox;
}

QWidget *UserCreatorGrantsSimpleLayout::createPrivilegesBox()
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

void UserCreatorGrantsSimpleLayout::checkAllQuickRoles()
{
    WidgetHelper::setCheckedBoxes(rolesBox, QStringList(), true, true);
}

void UserCreatorGrantsSimpleLayout::uncheckAllQuickRoles()
{
    WidgetHelper::setCheckedBoxes(rolesBox, QStringList(), false, true);
}

void UserCreatorGrantsSimpleLayout::checkAllQuickPrivs()
{
    WidgetHelper::setCheckedBoxes(privBox, QStringList(), true, true);
}

void UserCreatorGrantsSimpleLayout::uncheckAllQuickPrivs()
{
    WidgetHelper::setCheckedBoxes(privBox, QStringList(), false, true);
}

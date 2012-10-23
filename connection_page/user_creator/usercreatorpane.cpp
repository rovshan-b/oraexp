#include "usercreatorpane.h"
#include "usercreatortabs.h"
#include "connection_page/db_object_creator/dbobjectcreator.h"
#include <QtGui>

UserCreatorPane::UserCreatorPane(DbObjectCreator *objectCreator, QWidget *parent) :
    DbObjectCreatorPane(objectCreator, parent)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();
    mainLayout->setContentsMargins(0,0,0,0);

    userCreatorTabs=new UserCreatorTabs(objectCreator->getOriginalObjectName());

    mainLayout->addWidget(userCreatorTabs);
    setLayout(mainLayout);

    connect(userCreatorTabs, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    connect(userCreatorTabs, SIGNAL(objectInfoLoaded()), this, SIGNAL(objectInfoLoaded()));
}

void UserCreatorPane::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    DbObjectCreatorPane::setQueryScheduler(queryScheduler);

    userCreatorTabs->setQueryScheduler(queryScheduler);
}

QString UserCreatorPane::generateCreateDdl()
{
    return userCreatorTabs->generateCreateDdl();
}

QList<QueryListItem> UserCreatorPane::generateAlterDdl()
{
    return userCreatorTabs->generateAlterDdl();
}

bool UserCreatorPane::beforeAlter()
{
    return userCreatorTabs->beforeAlter();
}

QString UserCreatorPane::getSchemaName() const
{
    return "";
}

QString UserCreatorPane::getObjectName() const
{
    return userCreatorTabs->getUserName();
}

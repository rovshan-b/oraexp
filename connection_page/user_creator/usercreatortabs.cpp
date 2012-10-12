#include "usercreatortabs.h"
#include "tabs/usercreatorgeneralinfo.h"
#include "util/iconutil.h"
#include <QtGui>

UserCreatorTabs::UserCreatorTabs(const QString &objectName, QWidget *parent) :
    SubTabWidget(parent)
{
    editMode = !objectName.isEmpty();

    generalInfoTab = new UserCreatorGeneralInfo(objectName, this, editMode);
    //generalInfoTab->setMaximumWidth(350);
    QScrollArea *scrollAreaForGeneralInfoTab=new QScrollArea();
    scrollAreaForGeneralInfoTab->setWidget(generalInfoTab);
    scrollAreaForGeneralInfoTab->setWidgetResizable(true);
    addTab(scrollAreaForGeneralInfoTab, IconUtil::getIcon("user"), tr("General"));
}

void UserCreatorTabs::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    generalInfoTab->setQueryScheduler(queryScheduler);

    if(editMode){

    }else{
        emit objectInfoLoaded();
    }
}

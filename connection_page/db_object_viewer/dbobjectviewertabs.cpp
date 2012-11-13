#include "dbobjectviewertabs.h"
#include "dbobjectviewerwidget.h"
#include "dbobjectviewer.h"

DbObjectViewerTabs::DbObjectViewerTabs(const QString &schemaName,
                                 const QString &objectName,
                                 QWidget *parent) :
    SubTabWidget(parent),
    schemaName(schemaName),
    objectName(objectName),
    queryScheduler(0)
{   
}

void DbObjectViewerTabs::createUi()
{
    createTabs();
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(loadTabData(int)));
}

void DbObjectViewerTabs::loadTabData(int index)
{
    if(!queryScheduler){
        return;
    }

    Q_ASSERT(this->queryScheduler);

    DbObjectViewerWidget* selectedWidget=(DbObjectViewerWidget*)widget(index);

    if(!selectedWidget->areControlsCreated()){
        selectedWidget->createChildControls();
        selectedWidget->setQueryScheduler(this->queryScheduler);
        selectedWidget->setObjectName(schemaName, objectName);
        selectedWidget->loadInfo();
    }
}

void DbObjectViewerTabs::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    loadTabData(currentIndex());
}

IQueryScheduler *DbObjectViewerTabs::scheduler() const
{
    Q_ASSERT(this->queryScheduler);
    return this->queryScheduler;
}

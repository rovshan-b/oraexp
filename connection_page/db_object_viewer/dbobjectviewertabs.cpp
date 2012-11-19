#include "dbobjectviewertabs.h"
#include "dbobjectviewerwidget.h"
#include "dbobjectviewer.h"

DbObjectViewerTabs::DbObjectViewerTabs(const QString &schemaName,
                                 const QString &objectName,
                                 DbTreeModel::DbTreeNodeType itemType,
                                 DbUiManager *uiManager, QWidget *parent) :
    SubTabWidget(parent),
    schemaName(schemaName),
    objectName(objectName),
    itemType(itemType),
    uiManager(uiManager),
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
        selectedWidget->setObjectName(schemaName, objectName, itemType);
        selectedWidget->createChildControls(this->itemType, this->uiManager);
        selectedWidget->setQueryScheduler(this->queryScheduler);
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

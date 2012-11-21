#include "dbobjectviewertabs.h"
#include "dbobjectviewerwidget.h"
#include "dbobjectviewer.h"
#include "dbobjectviewertoolbar.h"
#include "widgets/subtabwidget.h"
#include "util/widgethelper.h"
#include "util/iconutil.h"
#include "context_menu/contextmenuutil.h"
#include <QtGui>

DbObjectViewerTabs::DbObjectViewerTabs(const QString &schemaName,
                                 const QString &objectName,
                                 DbTreeModel::DbTreeNodeType itemType,
                                 DbUiManager *uiManager, QWidget *parent) :
    QWidget(parent),
    schemaName(schemaName),
    objectName(objectName),
    itemType(itemType),
    uiManager(uiManager),
    queryScheduler(0),
    currentJobCount(0)
{   
}

void DbObjectViewerTabs::createUi()
{
    QVBoxLayout *layout=new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);

    toolbar=new DbObjectViewerToolBar();
    createToolbarButtons();
    layout->addWidget(toolbar);

    tabWidget=new SubTabWidget();
    layout->addWidget(tabWidget);

    createTabs();

    setLayout(layout);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(loadTabData(int)));
}

void DbObjectViewerTabs::addTab(DbObjectViewerWidget *tab, const QIcon &icon, const QString &title)
{
    tabWidget->addTab(tab, icon, title);
    connect(tab, SIGNAL(beforeLoadInfo()), this, SLOT(beforeLoadTabInfo()));
    connect(tab, SIGNAL(afterLoadInfo()), this, SLOT(afterLoadTabInfo()));
}

void DbObjectViewerTabs::loadTabData(int index)
{
    if(!queryScheduler){
        return;
    }

    setUpdatesEnabled(false);

    Q_ASSERT(this->queryScheduler);

    DbObjectViewerWidget* selectedWidget=(DbObjectViewerWidget*)tabWidget->widget(index);

    if(!selectedWidget->areControlsCreated()){
        selectedWidget->setObjectName(schemaName, objectName, itemType);
        selectedWidget->createChildControls();

        QList<QAction*> specificButtons=selectedWidget->getSpecificToolbarButtons();
        if(specificButtons.size()>0){
            tabSpecificActions[selectedWidget]=specificButtons;
            foreach(QAction* button, specificButtons){
                toolbar->insertAction(lastSeparatorBeforeProgressBar, button);
                button->setParent(this);
            }
        }

        selectedWidget->setQueryScheduler(this->queryScheduler);
        selectedWidget->loadInfo();
    }

    showTabSpecificActions(selectedWidget);

    setUpdatesEnabled(true);
}

void DbObjectViewerTabs::refreshInfo()
{
    OnDemandInfoViewerWidget *currentTab=qobject_cast<OnDemandInfoViewerWidget*>(tabWidget->currentWidget());
    Q_ASSERT(currentTab);
    currentTab->refreshInfo();
}

void DbObjectViewerTabs::beforeLoadTabInfo()
{
    ++currentJobCount;

    if(currentJobCount==1){ //disable only if actions are enabled
        toolbar->setEnabled(false);
        progressBarAction->setVisible(true);
        progressBarAction->setEnabled(true);
    }
}

void DbObjectViewerTabs::afterLoadTabInfo()
{
    Q_ASSERT(currentJobCount>0);

    if(--currentJobCount==0){
        progressBarAction->setVisible(false);
        toolbar->setEnabled(true);
    }
}

void DbObjectViewerTabs::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    loadTabData(tabWidget->currentIndex());
}

IQueryScheduler *DbObjectViewerTabs::scheduler() const
{
    Q_ASSERT(this->queryScheduler);
    return this->queryScheduler;
}

void DbObjectViewerTabs::createToolbarButtons()
{
    refreshButton=new QAction(IconUtil::getIcon("refresh"), tr("Refresh"), this);
    connect(refreshButton, SIGNAL(triggered()), this, SLOT(refreshInfo()));
    toolbar->addAction(refreshButton);

    //addSpecificToolbarButtons();

    lastSeparatorBeforeProgressBar=toolbar->addSeparator();

    QList<QAction*> actions=ContextMenuUtil::getActionsForObject(
                               this->schemaName, this->objectName,
                               itemType, uiManager);

    if(actions.size()>0){
        foreach(QAction *action, actions){
            toolbar->addAction(action);
            action->setParent(toolbar);
        }
        lastSeparatorBeforeProgressBar=toolbar->addSeparator();
    }

    progressBarAction=WidgetHelper::addProgressBarAction(toolbar);
    progressBarAction->setVisible(false);
}

void DbObjectViewerTabs::showTabSpecificActions(QWidget *currentTab)
{
    //show specific actions only when showing tab that added them
    QList<QWidget*> keys=tabSpecificActions.keys();
    foreach(QWidget *key, keys){
        QList<QAction*> specificButtons = tabSpecificActions.value(key, QList<QAction*>());
        foreach(QAction* button, specificButtons){
            button->setVisible(key==currentTab);
        }
    }
}

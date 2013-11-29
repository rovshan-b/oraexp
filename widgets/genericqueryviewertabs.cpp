#include "genericqueryviewertabs.h"
#include "widgets/subtabwidget.h"
#include "widgets/toolbar.h"
#include "widgets/genericqueryviewerwidget.h"
#include "util/widgethelper.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include <QtGui>

GenericQueryViewerTabs::GenericQueryViewerTabs(DbUiManager *uiManager, QWidget *parent) :
    QWidget(parent),
    uiManager(uiManager),
    queryScheduler(0),
    currentJobCount(0)
{
}

void GenericQueryViewerTabs::createUi()
{
    QVBoxLayout *layout=new QVBoxLayout();
    layout->setContentsMargins(0,2,0,0);

    toolbar=new ToolBar();
    createToolbarButtons();
    layout->addWidget(toolbar);

    tabWidget=new SubTabWidget();
    layout->addWidget(tabWidget);

    createTabs();

    setLayout(layout);

    connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
}

void GenericQueryViewerTabs::addTab(GenericQueryViewerWidget *tab, const QIcon &icon, const QString &title)
{
    tabWidget->addTab(tab, icon, title);
    connect(tab, SIGNAL(beforeLoadInfo()), this, SLOT(beforeLoadTabInfo()));
    connect(tab, SIGNAL(afterLoadInfo()), this, SLOT(afterLoadTabInfo()));
}

void GenericQueryViewerTabs::currentTabChanged(int index)
{
    showProgressBar();

    loadTabData(index);
}

void GenericQueryViewerTabs::loadTabData(int index)
{
    if(!queryScheduler){
        return;
    }

    setUpdatesEnabled(false);

    Q_ASSERT(this->queryScheduler);

    GenericQueryViewerWidget* selectedWidget=(GenericQueryViewerWidget*)tabWidget->widget(index);

    if(!selectedWidget->areControlsCreated()){
        initTab(selectedWidget);
    }

    showTabSpecificActions(selectedWidget);

    setUpdatesEnabled(true);

    if(selectedWidget->getNeedsRefresh()){
        selectedWidget->refreshInfo();
    }
}

void GenericQueryViewerTabs::refreshInfo()
{
    OnDemandInfoViewerWidget *currentTab=qobject_cast<OnDemandInfoViewerWidget*>(tabWidget->currentWidget());
    Q_ASSERT(currentTab);
    currentTab->refreshInfo();
}

void GenericQueryViewerTabs::beforeLoadTabInfo()
{
    ++currentJobCount;

    if(currentJobCount==1){ //disable only if actions are enabled
        toolbar->setEnabled(false);
        QTimer::singleShot(1000, this, SLOT(showProgressBar()));
    }
}

void GenericQueryViewerTabs::afterLoadTabInfo()
{
    Q_ASSERT(currentJobCount>0);

    if(--currentJobCount==0){
        progressBarAction->setVisible(false);
        toolbar->setEnabled(true);
    }
}

void GenericQueryViewerTabs::initTab(GenericQueryViewerWidget *tab)
{
    tab->createChildControls();

    QList<QAction*> specificButtons=tab->getSpecificToolbarButtons();
    if(specificButtons.size()>0){
        tabSpecificActions[tab]=specificButtons;
        foreach(QAction* button, specificButtons){
            toolbar->insertAction(lastSeparatorBeforeProgressBar, button);
            button->setParent(this);
        }
    }

    QList<QWidget*> specificWidgets=tab->getSpecificToolbarWidgets();
    if(specificWidgets.size()>0){
        tabSpecificWidgets[tab]=specificWidgets;
        foreach(QWidget* widget, specificWidgets){
            QAction *widgetAction = toolbar->insertWidget(lastSeparatorBeforeProgressBar, widget);
            tabSpecificActions[tab].append(widgetAction);
        }
    }

    tab->setQueryScheduler(this->queryScheduler);
    tab->loadInfo();
}

void GenericQueryViewerTabs::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;

    loadTabData(tabWidget->currentIndex());
}

IQueryScheduler *GenericQueryViewerTabs::scheduler() const
{
    Q_ASSERT(this->queryScheduler);
    return this->queryScheduler;
}

void GenericQueryViewerTabs::createToolbarButtons()
{
    createRefreshButton();

    lastSeparatorBeforeProgressBar=toolbar->addSeparator();

    createProgressBarAction();
}

void GenericQueryViewerTabs::createRefreshButton()
{
    refreshButton=new QAction(IconUtil::getIcon("refresh"), tr("Refresh"), this);
    connect(refreshButton, SIGNAL(triggered()), this, SLOT(refreshInfo()));
    toolbar->addAction(refreshButton);

    WidgetHelper::createAutoRefreshComboBox(toolbar, this);
}

void GenericQueryViewerTabs::createProgressBarAction()
{
    progressBarAction=WidgetHelper::addProgressBarAction(toolbar);
    progressBarAction->setVisible(false);
}

void GenericQueryViewerTabs::showTabSpecificActions(QWidget *currentTab)
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

void GenericQueryViewerTabs::showProgressBar()
{
    OnDemandInfoViewerWidget *currentTab=qobject_cast<OnDemandInfoViewerWidget*>(tabWidget->currentWidget());
    Q_ASSERT(currentTab);

    progressBarAction->setVisible(currentTab->isLoading());
    progressBarAction->setEnabled(currentTab->isLoading());
}

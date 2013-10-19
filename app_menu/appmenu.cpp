#include "appmenu.h"
#include "mainwindow.h"
#include "appfilemenu.h"
#include "appeditmenu.h"
#include "appviewmenu.h"
#include "apptoolsmenu.h"
#include "apphelpmenu.h"
#include "connectionspane.h"
#include "connection_page/connectionpage.h"
#include "connection_page/connectionpagetab.h"
#include "util/widgethelper.h"
#include <QtGui>

AppMenu *AppMenu::singleInstance=0;

AppMenu::AppMenu() : appFileMenu(0), allDisabled(false)
{
}

AppMenu::~AppMenu()
{
}

void AppMenu::cleanUp()
{
    delete AppMenu::singleInstance;
    qDebug("deleted instance of AppMenu");
}

AppMenu *AppMenu::defaultInstance()
{
    if(!AppMenu::singleInstance){
        AppMenu::singleInstance=new AppMenu();
        qDebug("created instance of AppMenu");
    }

    return AppMenu::singleInstance;
}

void AppMenu::setupMenu(MainWindow *mainWindow, ConnectionsPane *connectionsPane)
{
    this->connectionsPane=connectionsPane;
    connect(connectionsPane, SIGNAL(connectionPaneStateChanged()), this, SLOT(connectionsPaneStateChanged()));
    connect(connectionsPane, SIGNAL(connectionPageStateChanged()), this, SLOT(connectionsPaneStateChanged()));

    menuBar=mainWindow->menuBar();
    toolBar=new QToolBar(QObject::tr("Main toolbar"), mainWindow);
    toolBar->setIconSize(QSize(16,16));
    mainWindow->addToolBar(Qt::TopToolBarArea, toolBar);

    QMenu *fileMenu=menuBar->addMenu(QObject::tr("&File"));
    QMenu *editMenu=menuBar->addMenu(QObject::tr("&Edit"));
    QMenu *viewMenu=menuBar->addMenu(QObject::tr("&View"));
    QMenu *toolsMenu=menuBar->addMenu(QObject::tr("&Tools"));
    QMenu *helpMenu=menuBar->addMenu(QObject::tr("&Help"));

    appFileMenu=new AppFileMenu(fileMenu, toolBar, this);
    appEditMenu=new AppEditMenu(editMenu, toolBar, this);
    appViewMenu=new AppViewMenu(viewMenu, toolBar, this);
    appToolsMenu=new AppToolsMenu(toolsMenu, toolBar, this);
    appHelpMenu=new AppHelpMenu(helpMenu, toolBar, this);

    WidgetHelper::updateActionTooltips(toolBar);

    connectionsPaneStateChanged();
}

void AppMenu::disableAll()
{
    defaultInstance()->menuBar->setEnabled(false);
    defaultInstance()->toolBar->setEnabled(false);

    defaultInstance()->allDisabled = true;
}

bool AppMenu::isAllDisabled()
{
    return defaultInstance()->allDisabled;
}

void AppMenu::connectionsPaneStateChanged()
{
    if(allDisabled){
        return;
    }

    ConnectionPage *cnPage=this->connectionsPane->currentConnectionPage();
    ConnectionPageTab *cnPageTab=cnPage ? cnPage->currentConnectionPageTab() : 0;

    appFileMenu->updateActionStates(cnPage, cnPageTab);
    appEditMenu->focusWidgetChanged(0, qApp->focusWidget());
    appViewMenu->updateActionStates(cnPage, cnPageTab);
    appToolsMenu->updateActionStates(cnPage, cnPageTab);
}


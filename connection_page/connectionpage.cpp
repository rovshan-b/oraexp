#include "connectionpage.h"
#include "connectionpagetab.h"
#include "connectivity/dbconnection.h"
#include "info_panel/infopanel.h"
#include "navtree/treepane.h"
#include  "dbuimanager.h"
#include "util/iconutil.h"
#include "util/settingshelper.h"
#include <iostream>
#include <QtGui>

using namespace std;

ConnectionPage::ConnectionPage(DbConnection *db, QWidget *parent) :
    QMainWindow(parent), uiManager(db, this), stateChanged(false)
{
    this->db=db;

    //setWindowFlags(Qt::Widget);

    setDockNestingEnabled(true);

    treeDock=new QDockWidget(tr("Database objects"), this);
    treeDock->setObjectName("TreeViewDock");
    treeDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea);
    treeDock->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);

    treePane=new TreePane(&uiManager, treeDock);
    treePane->setConnection(db);

    treeDock->setWidget(treePane);
    addDockWidget(Qt::LeftDockWidgetArea, treeDock);

    centralTab=new TabWidget();
    centralTab->setTabsClosable(true);
    centralTab->setDocumentMode(true);
    centralTab->setMovable(true);

    setCentralWidget(centralTab);
    //QSplitter *splitter=new QSplitter(Qt::Horizontal);
    //splitter->addWidget(treePane);
    //splitter->addWidget(centralTab);
    //splitter->setStretchFactor(1, 2);

    //QHBoxLayout *layout=new QHBoxLayout();
    //layout->setContentsMargins(0,0,0,0);
    //layout->addWidget(splitter);
    //layout->addWidget(centralTab);
    //setLayout(layout);

    SettingsHelper::restoreMainWindowState(this, "ConnectionPage");

    uiManager.addWorksheet();

    connect(treeDock, SIGNAL(visibilityChanged(bool)), this, SIGNAL(connectionPageStateChanged()));
    connect(centralTab, SIGNAL(currentChanged(int)), this, SIGNAL(connectionPageStateChanged()));
    connect(centralTab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(&connectionPool, SIGNAL(asyncConnectionReady(DbConnection*,void*,bool,OciException)),
            this, SLOT(asyncConnectionReady(DbConnection*,void*,bool,OciException)));

    connectDockSignals(treeDock);
}

ConnectionPage::~ConnectionPage()
{
    delete centralTab; //to ensure child tabs are not using connection
    delete db;
}

DbUiManager *ConnectionPage::getUiManager()
{
    return &uiManager;
}

void ConnectionPage::closeTab(int index)
{
    QWidget *widgetToDelete=centralTab->widget(index);
    ConnectionPageTab *tabPage=dynamic_cast<ConnectionPageTab *>(widgetToDelete);

    if(tabPage==0){
        QMessageBox::critical(this->window(), tr("Wrong parent"),
                                 tr("Tabs must inherit from ConnectionPageTab"));
        return;
    }

    if(!tabPage->isBusy()){
        centralTab->removeTab(index);
        if(widgetToDelete!=0){
            widgetToDelete->deleteLater();
        }
    }else{
        QMessageBox::information(this->window(), tr("Tab busy"),
                                 tr("Cannot close this tab at the moment, because it is executing a database query"));
    }
}

void ConnectionPage::closeTab(QWidget *widget)
{
    int tabIx=centralTab->indexOf(widget);
    closeTab(tabIx);
}

void ConnectionPage::prepareTab(ConnectionPageTab *tab)
{
    connect(tab, SIGNAL(tabBusyStateChanged(ConnectionPageTab*,bool)), this, SLOT(tabBusyStateChanged(ConnectionPageTab*,bool)));

    if(tab->needsSeparateConnection()){
        connectionPool.requestConnection(this->db, tab);
    }else{
        //tab->setUpdatesEnabled(false);
        tab->setConnection(db);
        //tab->setUpdatesEnabled(true);
    }
}

void ConnectionPage::addTab(ConnectionPageTab *tab, const QPixmap &icon, const QString &title)
{
    tab->setEnabled(false);
    connect(tab, SIGNAL(initCompleted(ConnectionPageTab*)), this, SLOT(tabInitializationCompleted(ConnectionPageTab*)));

    tab->createUi();

    int newTabIx=centralTab->insertTab(centralTab->currentIndex()+1, tab, icon, title);
    //int newTabIx=centralTab->addTab(tab, icon, title);

    centralTab->setCurrentIndex(newTabIx);
    //centralTab->setTabBusy(newTabIx, true);
    connect(tab, SIGNAL(stateChanged()), this, SIGNAL(connectionPageStateChanged()));

    prepareTab(tab);
}

void ConnectionPage::asyncConnectionReady(DbConnection *db, void *data, bool error, const OciException &ex)
{
    ConnectionPageTab *tab = (ConnectionPageTab*)data;
    if(error){
        delete db;
        QMessageBox::critical(this->window(), tr("Error while connecting to database"), ex.getErrorMessage());
        //tab->decreaseRefCount();
    }
    else{
        tab->setUpdatesEnabled(false);
        tab->setConnection(db);
        tab->setUpdatesEnabled(true);
    }
}

void ConnectionPage::tabBusyStateChanged(ConnectionPageTab *tab, bool busy)
{
    centralTab->setTabBusy(tab, busy);
}

void ConnectionPage::tabInitializationCompleted(ConnectionPageTab *tab)
{
    tab->setEnabled(true);
    if(centralTab->currentWidget()==tab){
        tab->focusAvailable();
    }
}

ConnectionPageTab *ConnectionPage::currentConnectionPage() const
{
    ConnectionPageTab *cnPageTab=static_cast<ConnectionPageTab*>(centralTab->currentWidget());
    return cnPageTab;
}

void ConnectionPage::toggleTreePane()
{
    treeDock->setVisible(!treeDock->isVisible());
}

void ConnectionPage::windowStateChanged()
{
    if(!this->stateChanged){
        this->stateChanged=true;
    }
}

void ConnectionPage::connectDockSignals(QDockWidget *dockWidget)
{
    connect(dockWidget, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(windowStateChanged()));
    connect(dockWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(windowStateChanged()));
}

bool ConnectionPage::isTreePaneVisible() const
{
    return treeDock->isVisible();
}

void ConnectionPage::beforeClose()
{
    if(this->stateChanged){
        SettingsHelper::saveMainWindowState(this, "ConnectionPage");
    }
}

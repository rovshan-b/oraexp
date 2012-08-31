#include "connectionpage.h"
#include "connectionpagetab.h"
#include "connectivity/dbconnection.h"
#include "navtree/treepane.h"
#include  "dbuimanager.h"
#include <iostream>
#include <QtGui>

using namespace std;

ConnectionPage::ConnectionPage(DbConnection *db, QWidget *parent) :
    QWidget(parent), uiManager(db, this)
{
    this->db=db;

    setWindowFlags(Qt::Widget);

    treePane=new TreePane(&uiManager);
    treePane->setConnection(db);

    centralTab=new TabWidget();
    centralTab->setTabsClosable(true);
    centralTab->setDocumentMode(true);
    centralTab->setMovable(true);

    QSplitter *splitter=new QSplitter(Qt::Horizontal);
    splitter->addWidget(treePane);
    splitter->addWidget(centralTab);
    splitter->setStretchFactor(1, 2);

    QHBoxLayout *layout=new QHBoxLayout();
    layout->addWidget(splitter);
    setLayout(layout);

    uiManager.addWorksheet();

    connect(centralTab, SIGNAL(currentChanged(int)), this, SIGNAL(connectionPageStateChanged()));
    connect(centralTab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(&connectionPool, SIGNAL(asyncConnectionReady(DbConnection*,void*,bool,OciException)),
            this, SLOT(asyncConnectionReady(DbConnection*,void*,bool,OciException)));
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
            delete widgetToDelete;
        }
    }else{
        QMessageBox::information(this->window(), tr("Tab busy"),
                                 tr("Cannot close this tab at the moment, because it is currently busy performing "
                                    "an ongoing database operation"));
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

    int newTabIx=centralTab->addTab(tab, icon, title);

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

#include "connectionpageconnectedwidget.h"
#include "connectionpagetab.h"
#include "connectivity/dbconnection.h"
#include "info_panel/infopanel.h"
#include "navtree/treepane.h"
#include  "dbuimanager.h"
#include "util/iconutil.h"
#include "util/settingshelper.h"
#include "util/dialoghelper.h"
#include "util/appconnectionmanager.h"
#include "util/widgethelper.h"
#include "beans/ctrltabdata.h"
#include "connectionpagewindowobject.h"
#include "code_structure_tree/codestructurepane.h"
#include "widgets/dockwidget.h"
#include <iostream>
#include <QtGui>

using namespace std;

QByteArray ConnectionPageConnectedWidget::currentState;

ConnectionPageConnectedWidget::ConnectionPageConnectedWidget(DbConnection *db, DbUiManager *uiManager, QWidget *parent) :
    QMainWindow(parent), db(db), uiManager(uiManager), busyCounter(0)
{
    setDockNestingEnabled(true);

    createDbTreeDock();
    createCodeTreeDock();

    centralTab=new ConnectionPageTabWidget();
    centralTab->setTabsClosable(true);
    centralTab->setDocumentMode(true);
    centralTab->setMovable(true);

    setCentralWidget(centralTab);

    connect(centralTab, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
    connect(centralTab, SIGNAL(currentChanged(int)), this, SIGNAL(connectionPageStateChanged()));
    connect(centralTab, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(&connectionPool, SIGNAL(asyncConnectionReady(DbConnection*,void*,bool,OciException)),
            this, SLOT(asyncConnectionReady(DbConnection*,void*,bool,OciException)));
    connect(&connectionPool, SIGNAL(working(bool)), this, SLOT(connectionPoolWorking(bool)));

    new QShortcut(QKeySequence(QKeySequence::Close), this, SLOT(closeCurrentTab()));

    QTimer::singleShot(0, this, SLOT(restoreWindowState()));
}

void ConnectionPageConnectedWidget::createDbTreeDock()
{
    dbTreeDock=new DockWidget(tr("Database objects"), this);
    dbTreeDock->setObjectName("TreeViewDock");
    dbTreeDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea);
    dbTreeDock->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);

    dbTreePane=new TreePane(uiManager, dbTreeDock);
    childObjects.append(dbTreePane);
    dbTreePane->setDeleteListener(this);
    connect(dbTreePane, SIGNAL(busyStateChanged(ConnectionPageObject*,bool)), this, SLOT(tabBusyStateChanged(ConnectionPageObject*,bool)));
    dbTreePane->setConnection(db);
    AppConnectionManager::registerConnection(uiManager->getConnectionPage(), dbTreePane, db);

    dbTreeDock->setWidget(dbTreePane);
    addDockWidget(Qt::LeftDockWidgetArea, dbTreeDock);

    connect(dbTreeDock, SIGNAL(visibilityChanged(bool)), this, SIGNAL(connectionPageStateChanged()));
}

void ConnectionPageConnectedWidget::createCodeTreeDock()
{
    codeTreeDock = new DockWidget(tr("Code outline"), this);
    codeTreeDock->setObjectName("CodeStructureDock");
    codeTreeDock->setAllowedAreas(Qt::LeftDockWidgetArea |
                              Qt::RightDockWidgetArea);
    codeTreeDock->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable);

    codeTreePane=new CodeStructurePane(codeTreeDock);

    codeTreeDock->setWidget(codeTreePane);
    addDockWidget(Qt::LeftDockWidgetArea, codeTreeDock);

    connect(codeTreeDock, SIGNAL(visibilityChanged(bool)), this, SIGNAL(connectionPageStateChanged()));
}

ConnectionPageConnectedWidget::~ConnectionPageConnectedWidget()
{
    delete centralTab; //to ensure child tabs are not using connection
    AppConnectionManager::deleteConnection(db);

    //register backup connection if there is one so that it can be properly unregistered and disconnected
    DbConnection *backupConnection = connectionPool.getBackupConnection();
    if(backupConnection){
        AppConnectionManager::registerConnection(uiManager->getConnectionPage(), 0, backupConnection);
    }
}

/*
DbUiManager *ConnectionPageConnectedWidget::getUiManager()
{
    return &uiManager;
}*/

void ConnectionPageConnectedWidget::beforeClose()
{
    //call beforClose method of each child and then delete

    int childCount = childObjects.size();
    while(childCount-- > 0){
        ConnectionPageObject *obj = childObjects.at(0);
        obj->beforeClose();

        if(!obj->isWindow()){
            ConnectionPageTab *tab = dynamic_cast<ConnectionPageTab*>(obj);
            Q_ASSERT(tab);
            int ix = indexOf(tab);
            if(ix != -1){ //not tree pane
                centralTab->removeTab(ix);
            }
        }

        delete obj;
    }

    childObjects.clear();
}

void ConnectionPageConnectedWidget::closeTab(int index)
{
    QWidget *widgetToDelete=centralTab->widget(index);
    ConnectionPageTab *tabPage=dynamic_cast<ConnectionPageTab *>(widgetToDelete);

    if(tabPage==0){
        QMessageBox::critical(this->window(), tr("Wrong parent"),
                                 tr("Tabs must inherit from ConnectionPageTab"));
        return;
    }

    if(tabPage->isBusy()){
        QMessageBox::information(this->window(), tr("Tab busy"),
                                 tr("Cannot close tab while it is busy."));
        return;
    }

    if(tabPage->isModified()){
        int pressedButton = QMessageBox::question(this->window(), tr("Save changes?"),
                                                  tr("Do you want to save changes?"),
                                                  QMessageBox::Save|QMessageBox::Cancel|QMessageBox::Discard,
                                                  QMessageBox::Save);
        if(pressedButton==QMessageBox::Cancel){
            return;
        }else if(pressedButton==QMessageBox::Save && !tabPage->saveAll()){
            return;
        }
    }

    tabPage->beforeClose();
    centralTab->removeTab(index);
    if(widgetToDelete!=0){
        widgetToDelete->deleteLater();
    }
}

void ConnectionPageConnectedWidget::closeTab(QWidget *widget)
{
    int tabIx=centralTab->indexOf(widget);
    closeTab(tabIx);
}

void ConnectionPageConnectedWidget::prepareObject(ConnectionPageObject *obj)
{
    childObjects.append(obj);
    obj->setDeleteListener(this);

    QObject *objRef = dynamic_cast<QObject*>(obj);
    Q_ASSERT(objRef);
    connect(objRef, SIGNAL(busyStateChanged(ConnectionPageObject*,bool)), this, SLOT(tabBusyStateChanged(ConnectionPageObject*,bool)));

    if(obj->needsSeparateConnection()){
        obj->increaseRefCount();
        connectionPool.requestConnection(this->db, obj);
    }else{
        //tab->setUpdatesEnabled(false);
        obj->setConnection(db);
        //tab->setUpdatesEnabled(true);
    }
}

void ConnectionPageConnectedWidget::addTab(ConnectionPageTab *tab, const QPixmap &icon, const QString &title)
{
    tab->setTitle(title);
    tab->setEnabled(false);
    connect(tab, SIGNAL(initCompleted(ConnectionPageObject*)), this, SLOT(tabInitializationCompleted(ConnectionPageObject*)));

    tab->createUi();

    int newTabIx=centralTab->insertTab(centralTab->currentIndex()+1, tab, icon, title);
    //int newTabIx=centralTab->addTab(tab, icon, title);

    centralTab->setCurrentIndex(newTabIx);
    //centralTab->setTabBusy(newTabIx, true);
    connect(tab, SIGNAL(stateChanged()), this, SIGNAL(connectionPageStateChanged()));
    connect(tab, SIGNAL(captionChanged(ConnectionPageTab*,QString)), this, SLOT(changeTabCaption(ConnectionPageTab*,QString)));

    prepareObject(tab);
}

void ConnectionPageConnectedWidget::addWindow(ConnectionPageObject *window, const QPixmap &icon, const QString &title)
{
    window->createUi();

    QWidget *widget = dynamic_cast<QWidget*>(window);
    Q_ASSERT(widget);

    widget->setWindowIcon(icon.isNull() ? IconUtil::getIcon("database_big") : icon);
    widget->setWindowTitle(title);

    ConnectionPageWindowObject *windowObject = static_cast<ConnectionPageWindowObject*>(window);

    if(windowObject->initiallyVisible()){
        windowObject->showAndActivate();
    }

    prepareObject(window);
}

void ConnectionPageConnectedWidget::beforeDelete(ConnectionPageObject *obj)
{
    Q_ASSERT(obj);
    Q_ASSERT(childObjects.contains(obj));

    childObjects.removeOne(obj);

    Q_ASSERT(childObjects.size() >= 0);
}

void ConnectionPageConnectedWidget::asyncConnectionReady(DbConnection *db, void *data, bool error, const OciException &ex)
{
    ConnectionPageObject *obj = (ConnectionPageObject*)data;

    if(error){
        delete db;
        QMessageBox::critical(this->window(), tr("Error while connecting to database"), ex.getErrorMessage());
        //tab->decreaseRefCount();
    }else{
        QWidget *widget = dynamic_cast<QWidget*>(obj);

        Q_ASSERT(widget);

        widget->setUpdatesEnabled(false);
        obj->setConnection(db);
        AppConnectionManager::registerConnection(uiManager->getConnectionPage(), obj, db);
        widget->setUpdatesEnabled(true);
    }

    obj->decreaseRefCount();
}

void ConnectionPageConnectedWidget::tabBusyStateChanged(ConnectionPageObject *obj, bool busy)
{
    ConnectionPageTab *tab = dynamic_cast<ConnectionPageTab*>(obj);
    if(tab && tab!=dbTreePane){
        centralTab->setTabBusy(tab, busy);
    }

    if(busy){
        ++busyCounter;
    }else{
        --busyCounter;
        Q_ASSERT(busyCounter >= 0);
    }

    if((busyCounter == 0 && !busy) || (busyCounter == 1 && busy)){ //just got free or just got busy
        emit busyStateChanged(busy);
    }
}

void ConnectionPageConnectedWidget::tabInitializationCompleted(ConnectionPageObject *obj)
{
    ConnectionPageTab *tab = dynamic_cast<ConnectionPageTab*>(obj);

    if(tab){
        tab->setEnabled(true);
        if(centralTab->currentWidget()==tab){
            tab->focusAvailable();
        }
    }
}

void ConnectionPageConnectedWidget::currentTabChanged(int tabId)
{
    Q_UNUSED(tabId);
}

ConnectionPageTab *ConnectionPageConnectedWidget::currentConnectionPageTab() const
{
    ConnectionPageTab *cnPageTab=static_cast<ConnectionPageTab*>(centralTab->currentWidget());
    return cnPageTab;
}

int ConnectionPageConnectedWidget::indexOf(const ConnectionPageTab *tab) const
{
    for(int i=0; i<centralTab->count(); ++i){
        if(centralTab->widget(i) == tab){
            return i;
        }
    }

    return -1;
}

ConnectionPageTab *ConnectionPageConnectedWidget::tabAt(int index) const
{
    Q_ASSERT(index>=0 && index<centralTab->count());

    return static_cast<ConnectionPageTab*>(centralTab->widget(index));
}

QIcon ConnectionPageConnectedWidget::tabIcon(int index) const
{
    return centralTab->tabIcon(index);
}

QString ConnectionPageConnectedWidget::tabText(int index) const
{
    return centralTab->tabText(index);
}

bool ConnectionPageConnectedWidget::activateChildWidget(ConnectionPageObject *obj)
{
    ConnectionPageTab *cnPageTab = dynamic_cast<ConnectionPageTab*>(obj);

    if(!cnPageTab){
        return false;
    }

    if(cnPageTab == dbTreePane){
        toggleDbTreePane();

        return true;
    }

    /*if(cnPageTab == codeTreePane){
        toggleCodeTreePane();

        return true;
    }*/

    int ix = indexOf(cnPageTab);
    if(ix == -1){
        return false;
    }

    setCurrentIndex(ix);

    return true;
}

void ConnectionPageConnectedWidget::toggleDbTreePane()
{
    WidgetHelper::toggleDockWidget(dbTreeDock);
}

void ConnectionPageConnectedWidget::toggleCodeStructurePane()
{
    WidgetHelper::toggleDockWidget(codeTreeDock);
}

void ConnectionPageConnectedWidget::windowStateChanged()
{
    ConnectionPageConnectedWidget::currentState=saveState();
}

void ConnectionPageConnectedWidget::restoreWindowState()
{
    restoreState(ConnectionPageConnectedWidget::currentState);

    connectDockSignals(dbTreeDock);
    connectDockSignals(codeTreeDock);
    connect(codeTreeDock, SIGNAL(visibilityChanged(bool)), this, SLOT(codeTreePaneVisibilityChanged(bool)));

    if(ConnectionPageConnectedWidget::currentState.isEmpty()){
        tabifyDockWidget(dbTreeDock, codeTreeDock);
        dbTreeDock->raise();
    }

    //also add an empty worksheet
    //setting focus works correctly when adding from this slot
    uiManager->addWorksheet();

/*#ifdef DEBUG
    if(db->getUsername().compare("TEST_USER", Qt::CaseInsensitive)==0){
        uiManager->createEditor("TEST_USER", "TEST_PROC", "Procedure");
    }
#endif*/

}

void ConnectionPageConnectedWidget::changeTabCaption(ConnectionPageTab *tab, const QString &caption)
{
    centralTab->setTabText(centralTab->indexOf(tab), caption);
}

void ConnectionPageConnectedWidget::connectionPoolWorking(bool isWorking)
{
    tabBusyStateChanged(0, isWorking);
}

void ConnectionPageConnectedWidget::codeTreePaneVisibilityChanged(bool visible)
{
    codeTreePane->setActive(visible);
}

void ConnectionPageConnectedWidget::connectDockSignals(QDockWidget *dockWidget)
{
    connect(dockWidget, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(windowStateChanged()));
    connect(dockWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(windowStateChanged()));
}

bool ConnectionPageConnectedWidget::isDbTreePaneVisible() const
{
    return dbTreeDock->isVisible();
}

bool ConnectionPageConnectedWidget::isCodeStructurePaneVisible() const
{
    return codeTreeDock->isVisible();
}

QList<ConnectionPageTab *> ConnectionPageConnectedWidget::getTabsByType(const QString &className) const
{
    QList<ConnectionPageTab *> results;

    for(int i=0; i<centralTab->count(); ++i){
        QWidget *tab = centralTab->widget(i);
        ConnectionPageTab *cnPageTab=qobject_cast<ConnectionPageTab*>(tab);
        Q_ASSERT(cnPageTab);
        if(cnPageTab->metaObject()->className()==className){
            results.append(cnPageTab);
        }

    }

    return results;
}

QList<ConnectionPageTab *> ConnectionPageConnectedWidget::getTabsByConnection(DbConnection *db, const QString &className, int limit)
{
    QList<ConnectionPageTab *> results;

    for(int i=0; i<centralTab->count(); ++i){
        QWidget *tab = centralTab->widget(i);
        ConnectionPageTab *cnPageTab=qobject_cast<ConnectionPageTab*>(tab);
        Q_ASSERT(cnPageTab);
        if(cnPageTab->getDb()==db && (className.isEmpty() || cnPageTab->metaObject()->className()==className)){
            results.append(cnPageTab);
        }

        if(limit!=-1 && results.size()==limit){
            break;
        }
    }

    return results;
}

CodeStructurePane *ConnectionPageConnectedWidget::getCodeStructurePane() const
{
    return this->codeTreePane;
}

QList<CtrlTabData *> ConnectionPageConnectedWidget::getCtrlTabData() const
{
    QList<CtrlTabData *> results;

    QList<QWidget*> history = centralTab->getTabChangeHistory();
    foreach(QWidget *tab, history){
        int tabIndex = centralTab->indexOf(tab);
        results.append(new CtrlTabData(centralTab->tabIcon(tabIndex),
                                       centralTab->tabText(tabIndex),
                                       tab));
    }

    return results;
}

void ConnectionPageConnectedWidget::setCurrentTab(QWidget *widget)
{
    centralTab->setCurrentWidget(widget);
}

void ConnectionPageConnectedWidget::setCurrentIndex(int ix)
{
    centralTab->setCurrentIndex(ix);
}

ConnectionPageTab *ConnectionPageConnectedWidget::findTabById(const QString &tabId) const
{
    for(int i=0; i<centralTab->count(); ++i){
        QWidget *tab = centralTab->widget(i);
        ConnectionPageTab *cnPageTab=qobject_cast<ConnectionPageTab*>(tab);
        Q_ASSERT(cnPageTab);

        if(cnPageTab->getTabId() == tabId){
            return cnPageTab;
        }
    }

    return 0;
}

const ConnectionPool *ConnectionPageConnectedWidget::getConnectionPool() const
{
    return &this->connectionPool;
}

void ConnectionPageConnectedWidget::closeCurrentTab()
{
    int currentIx = centralTab->currentIndex();
    if(currentIx != -1){
        closeTab(currentIx);
    }
}

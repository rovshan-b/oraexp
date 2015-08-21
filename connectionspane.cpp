#include "connectionspane.h"
#include "connection_page/connectionpage.h"
#include "util/iconutil.h"
#include "connectivity/dbconnection.h"
#include "util/widgethelper.h"
#include "util/savechangesutil.h"
#include "util/savechangesutil.h"
#include "util/appconnectionmanager.h"
#include "dialogs/ctrltabdialog.h"
#include "dialogs/reconnectdialog.h"
#include "widgets/messagewidget.h"
#include "beans/dbconnectioninfo.h"
#include "app_menu/appmenu.h"
#include <QtGui>

ConnectionsPane::ConnectionsPane(QWidget *parent) :
    TabWidget(parent)
{
    setTabsClosable(true);
    setDocumentMode(true);
    setMovable(true);

    connect(this, SIGNAL(currentChanged(int)), this, SIGNAL(connectionPaneStateChanged()));
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    //addTab(new ConnectionPage(), IconUtil::getEnvironmentIcon(tr("Test")), "Example 1");
    setStyleSheet("QToolBar { border: 0px }");

    new QShortcut(QKeySequence("Ctrl+Tab"), this, SLOT(ctrlTabPressed()));

    addConnection();
}

void ConnectionsPane::addConnection(const QString &uuid)
{
    ConnectionPage *cnPage=new ConnectionPage(uuid);
    connect(cnPage, SIGNAL(busyStateChanged(ConnectionPage*,bool)), this, SLOT(tabBusyStateChanged(ConnectionPage*,bool)));
    connect(cnPage, SIGNAL(connectionPageStateChanged()), this, SIGNAL(connectionPageStateChanged()));
    connect(cnPage, SIGNAL(setTitle(QWidget*,DbConnectionInfo*)), this, SLOT(setTabTitle(QWidget*,DbConnectionInfo*)));
    setCurrentIndex(addTab(cnPage, IconUtil::getIcon("connect"), tr("Connect")));

    cnPage->focusReady();

    showTabBar();
}

ConnectionPage *ConnectionsPane::currentConnectionPage() const
{
    QWidget *w=currentWidget();
    if(w!=0){
        ConnectionPage *pane = static_cast<ConnectionPage*>(w);
        if(pane->isConnected()){
            return pane;
        }
    }

    return 0;
}

bool ConnectionsPane::isBusy() const
{
    for(int i=0; i<count(); ++i){
        ConnectionPage *pane = static_cast<ConnectionPage*>(widget(i));

        if(pane->isBusy()){
            return true;
        }
    }

    return false;
}

bool ConnectionsPane::activateChildWidget(ConnectionPage *cnPage, ConnectionPageObject *obj)
{
    int ix = indexOf(cnPage);

    if(ix == -1){
        return false;
    }

    if(obj->isWindow()){
        QWidget *w = dynamic_cast<QWidget*>(obj);
        Q_ASSERT(w);
        w->raise();

        return true;
    }

    setCurrentWidget(cnPage);

    return cnPage->activateChildWidget(obj);
}

void ConnectionsPane::ctrlTabPressed()
{
    ConnectionPage *cnPage = currentConnectionPage();
    if(!cnPage){
        return;
    }
    QList<CtrlTabData*> ctrlTabData = cnPage->getCtrlTabData();
    if(ctrlTabData.size()<2){
        return;
    }
    CtrlTabDialog dialog(ctrlTabData, this);
    if(dialog.exec()){
        cnPage->setCurrentTab(dialog.getSelectedWidget());
    }
}

void ConnectionsPane::setTabTitle(QWidget *tab, DbConnectionInfo *connectionInfo)
{
    int tabIx = indexOf(tab);

    setTabText(tabIx, connectionInfo->title);
    setTabIcon(tabIx, IconUtil::getEnvironmentIcon(connectionInfo->environment));

    updateMainWindowTitle();
}

void ConnectionsPane::tabBusyStateChanged(ConnectionPage *cnPage, bool busy)
{
    setTabBusy(cnPage, busy);
}

void ConnectionsPane::reconnect(OraExp::ReconnectMode reconnectMode)
{
    if(reconnectDialog){
        return;
    }

    reconnectDialog = new ReconnectDialog(this->window());
    QList< Triple<ConnectionPage *, ConnectionPageObject *, DbConnection*> * > connections;

    switch(reconnectMode){
    case OraExp::ReconnectModeAll:
        connections = AppConnectionManager::getAll();
        break;
    case OraExp::ReconnectModeCurrentConnection:
        connections = AppConnectionManager::getByConnectionPage(currentConnectionPage());
        break;
    case OraExp::ReconnectModeCurrentTab:
        connections = AppConnectionManager::getByConnectionPageObject(currentConnectionPage()->currentConnectionPageTab());
        break;
    }

    reconnectDialog->setConnections(connections);

    reconnectDialog->show();
    reconnectDialog->raise();
    reconnectDialog->activateWindow();
}

bool ConnectionsPane::closeAll(bool exiting)
{
    bool changesSaved = SaveChangesUtil::saveAll(this, true, true);

    if(changesSaved){
        //call beforeClose for each tab

        setUpdatesEnabled(false);

        while(count()>0){
            ConnectionPage *cnPage = qobject_cast<ConnectionPage*>(widget(0));
            cnPage->beforeClose();
            removeTab(0);
            delete cnPage;
        }

        setUpdatesEnabled(true);
    }

    if(exiting && changesSaved){
        QTimer::singleShot(0, this, SLOT(checkConnectionCountAndExit()));
    }

    return changesSaved;
}

void ConnectionsPane::checkConnectionCountAndExit()
{
    int connectionCount = AppConnectionManager::getActiveConnectionCount();

    if(connectionCount == 0){
        emit canExit();
        return;
    }

    if(reconnectDialog){
        reconnectDialog->hide();
    }

    connect(AppConnectionManager::defaultInstance(), SIGNAL(connectionDisconnected(DbConnection*)), this, SLOT(disconnected(DbConnection*)));

    bar()->hide();
    addTab(new MessageWidget(tr("Closing database connections (%1) ...").arg(connectionCount)), IconUtil::getIcon("exit"), tr("Exiting"));
    updateMainWindowTitle();
}

void ConnectionsPane::disconnected(DbConnection *db)
{
    Q_UNUSED(db);

    MessageWidget *w = static_cast<MessageWidget*>(widget(0));
    Q_ASSERT(w);

    int connectionCount = AppConnectionManager::getActiveConnectionCount();

    if(connectionCount == 0){
        emit canExit();
    }else{
        w->setMessage(tr("Closing database connections (%1) ...").arg(connectionCount));
    }
}

void ConnectionsPane::closeCurrentTab()
{
    int ix = currentIndex();
    if(ix!=-1){
        closeTab(ix);
    }
}

void ConnectionsPane::closeTab(int index)
{
    QWidget *widgetToDelete=widget(index);

    ConnectionPage *cnPage = qobject_cast<ConnectionPage*>(widgetToDelete);
    Q_ASSERT(cnPage);

    if(cnPage->isBusy()){
        QMessageBox::information(this->window(), tr("Connection busy"),
                                 tr("Cannot close connection while it is busy."));
        return;
    }

    if(!SaveChangesUtil::saveConnectionPage(this, index)){
        return;
    }

    cnPage->beforeClose();
    removeTab(index);
    if(widgetToDelete!=0){
        delete widgetToDelete;
    }

    showTabBar();
}

void ConnectionsPane::contextMenuEvent(QContextMenuEvent* pEvent){

  QTabBar* pTheBar = tabBar();
  Q_ASSERT(pTheBar != 0);

  int iCount = pTheBar->count();
  int i = 0;
  for ( ; i < iCount; ++i){
      if (pTheBar->tabRect(i).contains(pEvent->pos())) break;
  }

  // if no bar was hit, return
  if (i == iCount) {
      return ;
  }

  // ... otherwise  bring tab to front
  setCurrentIndex(i);

  // ... and launch popup
  QMenu qMenu(this);
  qMenu.addAction(IconUtil::getIcon("save"), "Save");
  qMenu.addAction(IconUtil::getIcon("pop_out"), "Pop out", this, SLOT(popOutTab()));

  qMenu.exec(QCursor::pos());
}

void ConnectionsPane::popOutTab()
{
    int currentIx=currentIndex();

    if(currentIx!=-1){
        WidgetHelper::popOutTab(this, currentIx);
    }

    showTabBar();
}

void ConnectionsPane::showTabBar()
{
    tabBar()->setVisible(count()>1);
    updateMainWindowTitle();
}

void ConnectionsPane::updateMainWindowTitle()
{
    QWidget *window=this->window();
    ConnectionPage *cnPage = qobject_cast<ConnectionPage*>(currentWidget());
    if(count()==1 && cnPage && cnPage->isConnected()){
        window->setWindowIcon(IconUtil::getEnvironmentIcon(cnPage->getConnectionEnvironment(), true));
        window->setWindowTitle(tabText(0)+" - OraExp");
    }else{
        window->setWindowIcon(IconUtil::getIcon("database_big"));
        window->setWindowTitle("OraExp");
    }
}

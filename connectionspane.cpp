#include "connectionspane.h"
#include "connection_page/connectionpage.h"
#include "util/iconutil.h"
#include "connectivity/dbconnection.h"
#include "util/widgethelper.h"
#include "util/savechangesutil.h"
#include "dialogs/ctrltabdialog.h"
#include "beans/dbconnectioninfo.h"
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
}

void ConnectionsPane::addConnection(const QString &uuid)
{
    QSharedPointer<ConnectionListModel> connectionListModel;

    for(int i=0; i<count(); ++i){
        ConnectionPage *page = static_cast<ConnectionPage*>(widget(i));
        if(!page->isConnected()){
            connectionListModel = page->getConnectionListModel();
            break;
        }

    }

    ConnectionPage *cnPage=new ConnectionPage(connectionListModel, uuid);
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
    if(count()==1){
        window->setWindowIcon(tabIcon(0));
        window->setWindowTitle(tabText(0)+" - Oracle Explorer");
    }else{
        window->setWindowIcon(IconUtil::getIcon("database"));
        window->setWindowTitle("Oracle Explorer");
    }
}

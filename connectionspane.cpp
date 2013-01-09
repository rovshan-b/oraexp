#include "connectionspane.h"
#include "connection_page/connectionpage.h"
#include "util/iconutil.h"
#include "connectivity/dbconnection.h"
#include "util/widgethelper.h"
#include <QtGui>

ConnectionsPane::ConnectionsPane(QWidget *parent) :
    QTabWidget(parent)
{
    setTabsClosable(true);
    setDocumentMode(true);
    setMovable(true);

    connect(this, SIGNAL(currentChanged(int)), this, SIGNAL(connectionPaneStateChanged()));
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    //addTab(new ConnectionPage(), IconUtil::getEnvironmentIcon(tr("Test")), "Example 1");
    setStyleSheet("QToolBar { border: 0px }");
}

void ConnectionsPane::addConnection(DbConnection *db)
{
    ConnectionPage *cnPage=new ConnectionPage(db);
    connect(cnPage, SIGNAL(connectionPageStateChanged()), this, SIGNAL(connectionPageStateChanged()));
    setCurrentIndex(
        addTab(cnPage, IconUtil::getEnvironmentIcon(db->getEnvironment()), db->getTitle())
    );

    showTabBar();
}

ConnectionPage *ConnectionsPane::currentConnectionPage() const
{
    QWidget *w=currentWidget();
    if(w!=0){
        return static_cast<ConnectionPage*>(w);
    }

    return 0;
}

void ConnectionsPane::closeTab(int index)
{
    QWidget *widgetToDelete=widget(index);
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
    int tabCount=count();
    tabBar()->setVisible(tabCount>1);
    QWidget *window=this->window();
    if(tabCount==1){
        window->setWindowIcon(tabIcon(0));
        window->setWindowTitle(tabText(0)+" - Oracle Explorer");
    }else{
        window->setWindowIcon(IconUtil::getIcon("database"));
        window->setWindowTitle("Oracle Explorer");
    }
}

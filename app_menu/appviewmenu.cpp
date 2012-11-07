#include "appviewmenu.h"
#include "connectionspane.h"
#include "connection_page/connectionpage.h"
#include "util/iconutil.h"
#include <QtGui>

AppViewMenu::AppViewMenu(QMenu *viewMenu, QToolBar *toolbar, QObject *parent) : AppMainMenu(parent)
{
    setupMenu(viewMenu, toolbar);
}

AppViewMenu::~AppViewMenu()
{
    delete viewAppStyleMenu;
}

void AppViewMenu::updateActionStates(ConnectionPage *cnPage, ConnectionPageTab * /*cnPageTab*/)
{
    viewDatabaseObjectsAction->setEnabled(cnPage!=0);
    viewDatabaseObjectsAction->setChecked(cnPage!=0 && cnPage->isTreePaneVisible());
}

void AppViewMenu::setupMenu(QMenu *viewMenu, QToolBar */*toolbar*/)
{
    viewDatabaseObjectsAction=viewMenu->addAction(IconUtil::getIcon("database"), tr("&Database objects"),
                                                  this, SLOT(toggleDbObjectsTree()), QKeySequence("Alt+0"));
    viewDatabaseObjectsAction->setStatusTip(tr("Show/Hide database objects pane"));
    viewDatabaseObjectsAction->setCheckable(true);
    viewDatabaseObjectsAction->setEnabled(false);

    viewApplicationStyleAction=viewMenu->addAction(tr("Style"));
    createAppStyleMenu();
    viewApplicationStyleAction->setMenu(viewAppStyleMenu);
}

void AppViewMenu::createAppStyleMenu()
{
    QString currentStyleName=qApp->style()->objectName();

    viewAppStyleMenu=new QMenu(tr("Application style"));
    QActionGroup *styleActions=new QActionGroup(viewAppStyleMenu);
    QAction *action;
    foreach(QString styleName, QStyleFactory::keys()){
        action=viewAppStyleMenu->addAction(styleName, this, SLOT(setApplicationStyle()));
        action->setData(styleName);
        action->setCheckable(true);
        styleActions->addAction(action);

        if(currentStyleName.compare(styleName, Qt::CaseInsensitive)==0){
            action->setChecked(true);
        }
    }
}

void AppViewMenu::setApplicationStyle()
{
    QAction *action=qobject_cast<QAction*>(sender());
    Q_ASSERT(action);
    qApp->setStyle(action->data().toString());
    action->setChecked(true);
}

void AppViewMenu::toggleDbObjectsTree()
{
    ConnectionPage *cnPage=getConnectionsPane()->currentConnectionPage();
    if(cnPage){
        cnPage->toggleTreePane();
    }
}

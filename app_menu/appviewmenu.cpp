#include "appviewmenu.h"
#include "connectionspane.h"
#include "connection_page/connectionpage.h"
#include "codeeditor/codeeditor.h"
#include "util/iconutil.h"
#include <QtGui>

AppViewMenu::AppViewMenu(QMenu *viewMenu, QToolBar *toolbar, QObject *parent) : AppMainMenu(parent)
{
    setupMenu(viewMenu, toolbar);
}

AppViewMenu::~AppViewMenu()
{
    //delete viewAppStyleMenu;
}

void AppViewMenu::updateActionStates(ConnectionPage *cnPage, ConnectionPageTab * /*cnPageTab*/)
{
    viewDatabaseObjectsAction->setEnabled(cnPage!=0);
    viewCodeStructureAction->setEnabled(cnPage!=0);

    viewDatabaseObjectsAction->setChecked(cnPage!=0 && cnPage->isDbTreePaneVisible());
    viewCodeStructureAction->setChecked(cnPage!=0 && cnPage->isCodeStructurePaneVisible());
}

void AppViewMenu::setupMenu(QMenu *viewMenu, QToolBar */*toolbar*/)
{
    viewDatabaseObjectsAction=viewMenu->addAction(IconUtil::getIcon("tree"), tr("&Database objects"),
                                                  this, SLOT(toggleDbObjectsTree()), QKeySequence("F12"));
    viewDatabaseObjectsAction->setStatusTip(tr("Show/Hide database objects pane"));
    viewDatabaseObjectsAction->setCheckable(true);
    viewDatabaseObjectsAction->setEnabled(false);

    viewCodeStructureAction=viewMenu->addAction(IconUtil::getIcon("ddl"), tr("&Code structure"),
                                                  this, SLOT(toggleCodeStructureTree()), QKeySequence("Ctrl+M"));
    viewCodeStructureAction->setStatusTip(tr("Show/Hide code structure for active code editor"));
    viewCodeStructureAction->setCheckable(true);
    viewCodeStructureAction->setEnabled(false);

    //viewApplicationStyleAction=viewMenu->addAction(tr("Style"));
    //createAppStyleMenu();
    //viewApplicationStyleAction->setMenu(viewAppStyleMenu);
}
/*
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
*/
void AppViewMenu::toggleDbObjectsTree()
{
    ConnectionPage *cnPage=getConnectionsPane()->currentConnectionPage();
    if(cnPage){
        cnPage->toggleDbTreePane();
    }
}

void AppViewMenu::toggleCodeStructureTree()
{
    ConnectionPage *cnPage=getConnectionsPane()->currentConnectionPage();
    if(cnPage){
        cnPage->toggleCodeStructurePane();
    }
}

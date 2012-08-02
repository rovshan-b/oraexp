#include "appsessionmenu.h"
#include "util/iconutil.h"
#include "util/dialoghelper.h"
#include <QtGui>

AppSessionMenu::AppSessionMenu(QMenu *sessionMenu, QToolBar *toolbar, QObject *parent) :
    AppMainMenu(parent)
{
    setupMenu(sessionMenu, toolbar);
}

void AppSessionMenu::setupMenu(QMenu *sessionMenu, QToolBar *toolbar)
{
    sessionConnectAction=sessionMenu->addAction(IconUtil::getIcon("connect"), tr("&Connect"), this, SLOT(showConnectDialog()));
    sessionConnectAction->setStatusTip(tr("Connect to database server"));

    toolbar->addSeparator();
    toolbar->addAction(sessionConnectAction);
}

void AppSessionMenu::showConnectDialog()
{
    DialogHelper::showConnectDialog(getConnectionsPane());
}

#include "appmainmenu.h"
#include "appmenu.h"
#include "connectionspane.h"
#include "connection_page/connectionpage.h"

AppMainMenu::AppMainMenu(QObject *parent) :
    QObject(parent)
{

}

ConnectionsPane *AppMainMenu::getConnectionsPane() const
{
    AppMenu *appMenu=static_cast<AppMenu*>(this->parent());
    return appMenu->getConnectionsPane();
}

DbUiManager *AppMainMenu::getUiManager() const
{
    AppMenu *appMenu=static_cast<AppMenu*>(this->parent());
    ConnectionPage *cnPage=appMenu->getConnectionsPane()->currentConnectionPage();

    if(!cnPage){
        return 0;
    }

    return cnPage->getUiManager();
}

DbUiManager *AppMainMenu::uiManager() const
{
    DbUiManager *uiManager=getUiManager();
    Q_ASSERT(uiManager);

    return uiManager;
}

ConnectionPageTab *AppMainMenu::currentConnectionPageTab() const
{
    AppMenu *appMenu=static_cast<AppMenu*>(this->parent());
    ConnectionPage *cnPage=appMenu->getConnectionsPane()->currentConnectionPage();
    Q_ASSERT(cnPage);
    ConnectionPageTab *cnPageTab=cnPage->currentConnectionPageTab();
    Q_ASSERT(cnPageTab);

    return cnPageTab;
}

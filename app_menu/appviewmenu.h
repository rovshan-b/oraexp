#ifndef APPVIEWMENU_H
#define APPVIEWMENU_H

#include "appmainmenu.h"

class QMenu;
class QToolBar;
class QAction;
class ConnectionPage;
class ConnectionPageTab;

class AppViewMenu : public AppMainMenu
{
    Q_OBJECT
public:
    explicit AppViewMenu(QMenu *viewMenu, QToolBar *toolbar, QObject *parent);
    ~AppViewMenu();

    void updateActionStates(ConnectionPage *cnPage, ConnectionPageTab *cnPageTab);

private slots:
    //void setApplicationStyle();
    void toggleDbObjectsTree();

private:
    void setupMenu(QMenu *viewMenu, QToolBar *toolbar);

    QAction *viewDatabaseObjectsAction;
    //QAction *viewApplicationStyleAction;

    //QMenu *viewAppStyleMenu;
    //void createAppStyleMenu();

};

#endif // APPVIEWMENU_H

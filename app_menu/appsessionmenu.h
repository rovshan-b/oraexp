#ifndef APPSESSIONMENU_H
#define APPSESSIONMENU_H

#include "appmainmenu.h"

class QMenu;
class QToolBar;
class QAction;

class AppSessionMenu : public AppMainMenu
{
    Q_OBJECT
public:
    explicit AppSessionMenu(QMenu *sessionMenu, QToolBar *toolbar, QObject *parent);

private slots:
    void showConnectDialog();

private:
    void setupMenu(QMenu *sessionMenu, QToolBar *toolbar);

    QAction *sessionConnectAction;

};

#endif // APPSESSIONMENU_H

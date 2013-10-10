#ifndef APPMAINMENU_H
#define APPMAINMENU_H

#include <QObject>

class ConnectionsPane;
class DbUiManager;
class ConnectionPageTab;

class AppMainMenu : public QObject
{
    Q_OBJECT
public:
    explicit AppMainMenu(QObject *parent = 0);
    virtual ~AppMainMenu(){}

    ConnectionsPane *getConnectionsPane() const;
    DbUiManager *getUiManager() const;
    DbUiManager *uiManager() const;
    ConnectionPageTab *currentConnectionPageTab() const;
};

#endif // APPMAINMENU_H

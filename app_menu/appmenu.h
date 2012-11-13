#ifndef APPMENU_H
#define APPMENU_H

#include <QObject>

class MainWindow;
class ConnectionsPane;
class QMenu;
class AppFileMenu;
class AppEditMenu;
class AppViewMenu;
class AppToolsMenu;
class AppHelpMenu;

class AppMenu : public QObject
{
    Q_OBJECT

public:
    ~AppMenu();

    void setupMenu(MainWindow *mainWindow, ConnectionsPane *connectionsPane);
    static void cleanUp();

    static AppMenu *defaultInstance();

    ConnectionsPane *getConnectionsPane() const {return this->connectionsPane;}

    AppEditMenu *getEditMenu() const {return this->appEditMenu;}

private slots:
    void connectionsPaneStateChanged();

private:
    AppMenu();

    static AppMenu *singleInstance;

    ConnectionsPane *connectionsPane;

    AppFileMenu *appFileMenu;
    AppEditMenu *appEditMenu;
    AppViewMenu *appViewMenu;
    AppToolsMenu *appToolsMenu;
    AppHelpMenu *appHelpMenu;
};

#endif // APPMENU_H

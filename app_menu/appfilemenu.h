#ifndef APPFILEMENU_H
#define APPFILEMENU_H

#include "appmainmenu.h"
#include "connection_page/dbuimanager.h"
#include "connection_page/connectionpagetab.h"
#include "util/savechangesutil.h"

class QMenu;
class QToolBar;
class QAction;
class ConnectionPageTab;
class DbConnectionInfo;

class AppFileMenu : public AppMainMenu
{
    Q_OBJECT

public:
    AppFileMenu(QMenu *fileMenu, QToolBar *toolbar, QObject *parent);
    ~AppFileMenu();

    void updateActionStates(ConnectionPage *cnPage, ConnectionPageTab *cnPageTab);

    void addToRecentFileList(const QString &fileName);
    void saveRecentFileList();

private slots:
    void showConnectDialog();
    void reconnectAll();
    void reconnectCurrentConnection();
    void reconnectCurrentTab();

    void enableReconnectMenuActions();

    void addWorksheet(){uiManager()->addWorksheet();}
    void showCreator();

    void open(){uiManager()->openFiles();}
    void openRecent();
    void save(){currentConnectionPageTab()->saveContents();}
    void saveAs(){currentConnectionPageTab()->saveContentsAs();}
    void saveAll(){SaveChangesUtil::saveAll(getConnectionsPane(), false, false);}

    void populateConnectionMenu();

    void exitApplication();

    QList<DbConnectionInfo*> getConnectionsByEnvironment(QList<DbConnectionInfo*> allConnections, OraExp::ConnectionEnvironment environment);

private:
    void setupMenu(QMenu *fileMenu, QToolBar *toolbar);

    QAction *fileConnectAction;
    QAction *fileConnectToolbarAction;

    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *fileOpenToolbarAction;
    QAction *fileOpenRecentAction;
    QAction *fileCloseAction;
    QAction *fileCloseAllAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *fileSaveAllAction;
    QAction *filePrintAction;
    QAction *filePrintPreviewAction;

    QMenu *connectionListMenu;
    void createConnectionListMenu();

    QMenu *reconnectMenu;

    QMenu *fileNewMenu;
    void createFileNewMenu();

    QMenu *recentFilesMenu;
    void createRecentFilesMenu();
};

#endif // APPFILEMENU_H

#ifndef APPFILEMENU_H
#define APPFILEMENU_H

#include "appmainmenu.h"
#include "connection_page/dbuimanager.h"
#include "connection_page/connectionpagetab.h"

class QMenu;
class QToolBar;
class QAction;
class ConnectionPageTab;

class AppFileMenu : public AppMainMenu
{
    Q_OBJECT

public:
    AppFileMenu(QMenu *fileMenu, QToolBar *toolbar, QObject *parent);
    ~AppFileMenu();

    void updateActionStates(ConnectionPage *cnPage, ConnectionPageTab *cnPageTab);

private slots:
    void addWorksheet(){uiManager()->addWorksheet();}
    void showTableCreator(){uiManager()->showTableCreator();}
    void showViewCreator(){uiManager()->showViewCreator();}

    void open(){currentConnectionPageTab()->open();}

private:
    void setupMenu(QMenu *fileMenu, QToolBar *toolbar);

    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *fileCloseAction;
    QAction *fileCloseAllAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *fileSaveAllAction;
    QAction *filePrintAction;
    QAction *filePrintPreviewAction;

    QMenu *fileNewMenu;
    void createFileNewMenu();
};

#endif // APPFILEMENU_H

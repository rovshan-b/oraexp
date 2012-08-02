#ifndef APPEDITMENU_H
#define APPEDITMENU_H

#include "appmainmenu.h"
#include "connection_page/dbuimanager.h"
#include "connection_page/connectionpagetab.h"

class QMenu;
class QToolBar;
class QAction;
class ConnectionPage;
class ConnectionPageTab;

class AppEditMenu : public AppMainMenu
{
    Q_OBJECT
public:
    explicit AppEditMenu(QMenu *editMenu, QToolBar *toolbar, QObject *parent);

    void updateActionStates(ConnectionPage *cnPage, ConnectionPageTab *cnPageTab);

private slots:
    void focusWidgetChanged(QWidget *old, QWidget *now);

    void undo();
    void redo();

    void cut();
    void copy();
    void paste();

    void showSearchWidget(){currentConnectionPageTab()->showSearchWidget();}
    void findNext(){currentConnectionPageTab()->findNext();}
    void findPrevious(){currentConnectionPageTab()->findPrevious();}

private:
    void setupMenu(QMenu *editMenu, QToolBar *toolbar);

    QAction *editUndoAction;
    QAction *editRedoAction;
    QAction *editCutAction;
    QAction *editCopyAction;
    QAction *editPasteAction;
    QAction *editFindAction;
    QAction *editFindNextAction;
    QAction *editFindPreviousAction;

    QWidget *currentAppWidget;

};

#endif // APPEDITMENU_H

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
class CodeEditor;

class AppEditMenu : public AppMainMenu
{
    Q_OBJECT
public:
    explicit AppEditMenu(QMenu *editMenu, QToolBar *toolbar, QObject *parent);

    void updateActionStates(ConnectionPage *cnPage, ConnectionPageTab *cnPageTab);

    void updateActionStatesForCodeEditor(CodeEditor *editor);
//    void setUndoEnabled(bool enabled);
//    void setRedoEnabled(bool enabled);

private slots:
    void focusWidgetChanged(QWidget *old, QWidget *now);

    void undo();
    void redo();

    void cut();
    void copy();
    void paste();

    void comment();
    void moveUp();
    void moveDown();
    void selectBlock();
    void increaseFont();
    void decreaseFont();

    void toUpperCase();
    void toLowerCase();
    void makeDuplicate();

    void showSearchWidget(){currentConnectionPageTab()->showSearchWidget();}
    void findNext(){currentConnectionPageTab()->findNext();}
    void findPrevious(){currentConnectionPageTab()->findPrevious();}

    void goToLine();

private:
    void setupMenu(QMenu *editMenu, QToolBar *toolbar);

    QAction *editUndoAction;
    QAction *editRedoAction;
    QAction *editCutAction;
    QAction *editCopyAction;
    QAction *editPasteAction;

    QAction *editCommentAction;
    QAction *editMoveUpAction;
    QAction *editMoveDownAction;
    QAction *editSelectBlockAction;
    QAction *editToUpperCaseAction;
    QAction *editToLowerCaseAction;
    QAction *editCreateDuplicateAction;

    QAction *editIncreaseFontSize;
    QAction *editDecreaseFontSize;

    QAction *editFindAction;
    QAction *editFindNextAction;
    QAction *editFindPreviousAction;

    QAction *editGoToLineAction;

    QWidget *currentAppWidget;

};

#endif // APPEDITMENU_H

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

public slots:
    void setUndoEnabled(bool enabled);
    void setRedoEnabled(bool enabled);
    void focusWidgetChanged(QWidget *old, QWidget *now);

private slots:

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
    void resetFont();

    void toUpperCase();
    void toLowerCase();
    void makeDuplicate();
    void removeEmptyLines();

    void showSearchWidget();
    void findNext();
    void findPrevious();

    void goToLine();

    void updateActionStatesForCodeEditor(CodeEditor *editor);

public:
    QAction *editUndoAction;
    QAction *editRedoAction;
    QAction *editCutAction;
    QAction *editCopyAction;
    //QAction *editCopyAsAction;
    QAction *editPasteAction;

    QAction *editCommentAction;
    QAction *editMoveUpAction;
    QAction *editMoveDownAction;
    QAction *editSelectBlockAction;
    QAction *editToUpperCaseAction;
    QAction *editToLowerCaseAction;
    QAction *editCreateDuplicateAction;
    QAction *editRemoveEmptyLinesAction;

private:
    void setupMenu(QMenu *editMenu, QToolBar *toolbar);
    QMenu *createCopyAsMenu(QWidget *parent);

    QAction *editIncreaseFontSize;
    QAction *editDecreaseFontSize;
    QAction *editResetFontSize;

    QAction *editFindAction;
    QAction *editFindNextAction;
    QAction *editFindPreviousAction;

    QAction *editGoToLineAction;

    QWidget *currentAppWidget;

    QWidget *findParentSearchPane() const;

};

#endif // APPEDITMENU_H

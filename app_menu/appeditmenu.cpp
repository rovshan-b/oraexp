#include "appeditmenu.h"
#include "util/iconutil.h"
#include "connection_page/connectionpage.h"
#include "connection_page/connectionpagetab.h"
#include "codeeditor/codeeditor.h"
#include "util/widgethelper.h"
#include <QtGui>

AppEditMenu::AppEditMenu(QMenu *editMenu, QToolBar *toolbar, QObject *parent) : AppMainMenu(parent), currentAppWidget(0)
{
    setupMenu(editMenu, toolbar);

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(focusWidgetChanged(QWidget*,QWidget*)));
}

void AppEditMenu::setupMenu(QMenu *editMenu, QToolBar *toolbar)
{
    toolbar->addSeparator();

    editUndoAction=editMenu->addAction(IconUtil::getIcon("undo"), tr("&Undo"), this, SLOT(undo()), QKeySequence(QKeySequence::Undo));
    editUndoAction->setStatusTip(tr("Undo last edit action"));
    toolbar->addAction(editUndoAction);
    editRedoAction=editMenu->addAction(IconUtil::getIcon("redo"), tr("&Redo"), this, SLOT(redo()), QKeySequence(QKeySequence::Redo));
    editRedoAction->setStatusTip(tr("Redo last undone edit action"));
    toolbar->addAction(editRedoAction);

    editMenu->addSeparator();
    toolbar->addSeparator();

    editCutAction=editMenu->addAction(IconUtil::getIcon("editcut"), tr("Cu&t"), this, SLOT(cut()), QKeySequence(QKeySequence::Cut));
    editCutAction->setStatusTip(tr("Cut"));
    toolbar->addAction(editCutAction);
    editCopyAction=editMenu->addAction(IconUtil::getIcon("editcopy"), tr("&Copy"), this, SLOT(copy()), QKeySequence(QKeySequence::Copy));
    editCopyAction->setStatusTip(tr("Copy"));
    toolbar->addAction(editCopyAction);
    editPasteAction=editMenu->addAction(IconUtil::getIcon("editpaste"), tr("&Paste"), this, SLOT(paste()), QKeySequence(QKeySequence::Paste));
    editPasteAction->setStatusTip(tr("Paste"));
    toolbar->addAction(editPasteAction);

    editMenu->addSeparator();
    toolbar->addSeparator();

    editCommentAction=editMenu->addAction(IconUtil::getIcon("comment"), tr("Co&mment/Uncomment"), this, SLOT(comment()), QKeySequence("Ctrl+/"));
    editCommentAction->setStatusTip(tr("Comment/Uncomment line(s)"));
    toolbar->addAction(editCommentAction);

    editMoveUpAction=editMenu->addAction(IconUtil::getIcon("move_up"), tr("Move &up"), this, SLOT(moveUp()), QKeySequence("Ctrl+["));
    editMoveUpAction->setStatusTip(tr("Move lines(s) up"));
    toolbar->addAction(editMoveUpAction);

    editMoveDownAction=editMenu->addAction(IconUtil::getIcon("move_down"), tr("Move &down"), this, SLOT(moveDown()), QKeySequence("Ctrl+]"));
    editMoveDownAction->setStatusTip(tr("Move lines(s) down"));
    toolbar->addAction(editMoveDownAction);

    editSelectBlockAction=editMenu->addAction(tr("Select current block"), this, SLOT(selectBlock()), QKeySequence("Ctrl+B"));
    editSelectBlockAction->setStatusTip(tr("Select current block"));

    editToUpperCaseAction=editMenu->addAction(tr("To upper case"), this, SLOT(toUpperCase()), QKeySequence("Ctrl+U"));
    editToUpperCaseAction->setStatusTip(tr("Change selection or current word text to upper case"));

    editToLowerCaseAction=editMenu->addAction(tr("To lower case"), this, SLOT(toLowerCase()), QKeySequence("Ctrl+L"));
    editToLowerCaseAction->setStatusTip(tr("Change selection or current word text to lower case"));

    editCreateDuplicateAction=editMenu->addAction(tr("Make duplicate"), this, SLOT(makeDuplicate()), QKeySequence("Ctrl+D"));
    editCreateDuplicateAction->setStatusTip(tr("Create duplicate of current line or selection"));

    //toolbar->addAction(editSelectBlockAction);

    editMenu->addSeparator();
    //toolbar->addSeparator();

    editIncreaseFontSize=editMenu->addAction(tr("Increase font size"), this, SLOT(increaseFont()), QKeySequence(QKeySequence::ZoomIn));
    editIncreaseFontSize->setStatusTip(tr("Increase font size"));

    editDecreaseFontSize=editMenu->addAction(tr("Decrease font size"), this, SLOT(decreaseFont()), QKeySequence(QKeySequence::ZoomOut));
    editDecreaseFontSize->setStatusTip(tr("Decrease font size"));

    editMenu->addSeparator();
    toolbar->addSeparator();

    editFindAction=editMenu->addAction(IconUtil::getIcon("find"), tr("&Find..."), this, SLOT(showSearchWidget()), QKeySequence("Ctrl+F"));
    editFindAction->setStatusTip(tr("Find text"));
    toolbar->addAction(editFindAction);
    editFindPreviousAction=editMenu->addAction(IconUtil::getIcon("find_prev"), tr("Find p&revious"), this, SLOT(findPrevious()), QKeySequence("Shift+F3"));
    editFindPreviousAction->setStatusTip(tr("Find previous occurence of current search text"));
    toolbar->addAction(editFindPreviousAction);
    editFindNextAction=editMenu->addAction(IconUtil::getIcon("find_next"), tr("Find &next"), this, SLOT(findNext()), QKeySequence("F3"));
    editFindNextAction->setStatusTip(tr("Find next occurence of current search text"));
    toolbar->addAction(editFindNextAction);

    editMenu->addSeparator();

    editGoToLineAction=editMenu->addAction(IconUtil::getIcon("right_arrow"), tr("&Go to line..."), this, SLOT(goToLine()), QKeySequence("Ctrl+G"));
    editGoToLineAction->setStatusTip(tr("Go to line"));

}

void AppEditMenu::updateActionStates(ConnectionPage * /*cnPage*/, ConnectionPageTab *cnPageTab)
{
    editFindAction->setEnabled(cnPageTab!=0 && cnPageTab->canFind());
    editFindNextAction->setEnabled(cnPageTab!=0 && cnPageTab->canFindNext());
    editFindPreviousAction->setEnabled(cnPageTab!=0 && cnPageTab->canFindPrevious());
}

void AppEditMenu::updateActionStatesForCodeEditor(CodeEditor *editor)
{
    Q_ASSERT(editor);
    bool isReadOnly=editor->isReadOnly();

    editUndoAction->setEnabled(!isReadOnly);
    editRedoAction->setEnabled(!isReadOnly);
    editCutAction->setEnabled(!isReadOnly);
    editCopyAction->setEnabled(true);
    editPasteAction->setEnabled(!isReadOnly);

    editCommentAction->setEnabled(!isReadOnly);
    editMoveUpAction->setEnabled(!isReadOnly);
    editMoveDownAction->setEnabled(!isReadOnly);
    editSelectBlockAction->setEnabled(!isReadOnly);
    editToUpperCaseAction->setEnabled(!isReadOnly);
    editToLowerCaseAction->setEnabled(!isReadOnly);
    editCreateDuplicateAction->setEnabled(!isReadOnly);

    editIncreaseFontSize->setEnabled(true);
    editDecreaseFontSize->setEnabled(true);

    editGoToLineAction->setEnabled(true);
}
/*
void AppEditMenu::setUndoEnabled(bool enabled)
{
    editUndoAction->setEnabled(enabled);
}

void AppEditMenu::setRedoEnabled(bool enabled)
{
    editRedoAction->setEnabled(enabled);
}*/

void AppEditMenu::focusWidgetChanged(QWidget * /*old*/, QWidget *now)
{
    if(now==0){
        return;
    }

    currentAppWidget=now;

    CodeEditor *editor = qobject_cast<CodeEditor*>(currentAppWidget);
    bool isCodeEditor=(editor!=0);
    if(isCodeEditor){ //code editor will update action states by direct calls
        return;
    }

    bool canUndo=now->metaObject()->indexOfSlot("undo()")>-1;
    bool canRedo=now->metaObject()->indexOfSlot("redo()")>-1;

    editUndoAction->setEnabled(canUndo);
    editRedoAction->setEnabled(canRedo);

    bool canCut=now->metaObject()->indexOfSlot("cut()")>-1;
    bool canCopy=now->metaObject()->indexOfSlot("copy()")>-1;
    bool canPaste=now->metaObject()->indexOfSlot("paste()")>-1;

    editCutAction->setEnabled(canCut);
    editCopyAction->setEnabled(canCopy);
    editPasteAction->setEnabled(canPaste);

    editCommentAction->setEnabled(false);
    editMoveUpAction->setEnabled(false);
    editMoveDownAction->setEnabled(false);
    editSelectBlockAction->setEnabled(false);
    editToUpperCaseAction->setEnabled(false);
    editToLowerCaseAction->setEnabled(false);
    editCreateDuplicateAction->setEnabled(false);

    editIncreaseFontSize->setEnabled(false);
    editDecreaseFontSize->setEnabled(false);

    editGoToLineAction->setEnabled(false);
}

void AppEditMenu::undo()
{
    WidgetHelper::invokeSlot(currentAppWidget, "undo");
}

void AppEditMenu::redo()
{
    WidgetHelper::invokeSlot(currentAppWidget, "redo");
}

void AppEditMenu::cut()
{
    WidgetHelper::invokeSlot(currentAppWidget, "cut");
}

void AppEditMenu::copy()
{
    WidgetHelper::invokeSlot(currentAppWidget, "copy");
}

void AppEditMenu::paste()
{
    WidgetHelper::invokeSlot(currentAppWidget, "paste");
}

void AppEditMenu::comment()
{
    WidgetHelper::invokeSlot(currentAppWidget, "commentBlocks");
}

void AppEditMenu::moveUp()
{
    WidgetHelper::invokeSlot(currentAppWidget, "moveUp");
}

void AppEditMenu::moveDown()
{
    WidgetHelper::invokeSlot(currentAppWidget, "moveDown");
}

void AppEditMenu::selectBlock()
{
    WidgetHelper::invokeSlot(currentAppWidget, "selectCurrentBlock");
}

void AppEditMenu::increaseFont()
{
    WidgetHelper::invokeSlot(currentAppWidget, "increaseFontSize");
}

void AppEditMenu::decreaseFont()
{
    WidgetHelper::invokeSlot(currentAppWidget, "decreaseFontSize");
}

void AppEditMenu::toUpperCase()
{
    WidgetHelper::invokeSlot(currentAppWidget, "toUpperCase");
}

void AppEditMenu::toLowerCase()
{
    WidgetHelper::invokeSlot(currentAppWidget, "toLowerCase");
}

void AppEditMenu::makeDuplicate()
{
    WidgetHelper::invokeSlot(currentAppWidget, "makeDuplicate");
}

void AppEditMenu::goToLine()
{
    if(!currentAppWidget){
        return;
    }

    currentAppWidget->metaObject()->invokeMethod(currentAppWidget, "goToLine", Qt::DirectConnection);
}

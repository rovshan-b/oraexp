#include "appeditmenu.h"
#include "util/iconutil.h"
#include "connection_page/connectionpage.h"
#include "connection_page/connectionpagetab.h"
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

    editFindAction=editMenu->addAction(IconUtil::getIcon("find"), tr("&Find..."), this, SLOT(showSearchWidget()), QKeySequence(QKeySequence::Find));
    editFindAction->setStatusTip(tr("Find text"));
    toolbar->addAction(editFindAction);
    editFindPreviousAction=editMenu->addAction(IconUtil::getIcon("find_prev"), tr("Find p&revious"), this, SLOT(findPrevious()), QKeySequence(QKeySequence::FindPrevious));
    editFindPreviousAction->setStatusTip(tr("Find previous occurence of current search text"));
    toolbar->addAction(editFindPreviousAction);
    editFindNextAction=editMenu->addAction(IconUtil::getIcon("find_next"), tr("Find &next"), this, SLOT(findNext()), QKeySequence(QKeySequence::FindNext));
    editFindNextAction->setStatusTip(tr("Find next occurence of current search text"));
    toolbar->addAction(editFindNextAction);

}

void AppEditMenu::updateActionStates(ConnectionPage * /*cnPage*/, ConnectionPageTab *cnPageTab)
{
    editFindAction->setEnabled(cnPageTab!=0 && cnPageTab->canFind());
    editFindNextAction->setEnabled(cnPageTab!=0 && cnPageTab->canFindNext());
    editFindPreviousAction->setEnabled(cnPageTab!=0 && cnPageTab->canFindPrevious());
}

void AppEditMenu::focusWidgetChanged(QWidget * /*old*/, QWidget *now)
{
    if(now==0){
        return;
    }

    currentAppWidget=now;

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
}

void AppEditMenu::undo()
{
    if(!currentAppWidget){
        return;
    }

    currentAppWidget->metaObject()->invokeMethod(currentAppWidget, "undo", Qt::DirectConnection);
}

void AppEditMenu::redo()
{
    if(!currentAppWidget){
        return;
    }

    currentAppWidget->metaObject()->invokeMethod(currentAppWidget, "redo", Qt::DirectConnection);
}

void AppEditMenu::cut()
{
    if(!currentAppWidget){
        return;
    }

    currentAppWidget->metaObject()->invokeMethod(currentAppWidget, "cut", Qt::DirectConnection);
}

void AppEditMenu::copy()
{
    if(!currentAppWidget){
        return;
    }

    currentAppWidget->metaObject()->invokeMethod(currentAppWidget, "copy", Qt::DirectConnection);
}

void AppEditMenu::paste()
{
    if(!currentAppWidget){
        return;
    }

    currentAppWidget->metaObject()->invokeMethod(currentAppWidget, "paste", Qt::DirectConnection);
}

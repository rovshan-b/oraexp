#include "appeditmenu.h"
#include "util/iconutil.h"
#include "util/widgethelper.h"
#include "util/codeeditorutil.h"
#include "util/dbutil.h"
#include "util/objectresolver.h"
#include "context_menu/contextmenuutil.h"
#include "connectionspane.h"
#include "connection_page/connectionpage.h"
#include "connection_page/connectionpagetab.h"
#include "codeeditor/codeeditor.h"
#include "mainwindow.h"
#include "errors.h"
#include <QtGui>

AppEditMenu::AppEditMenu(QMenu *editMenu, QToolBar *toolbar, QObject *parent) : AppMainMenu(parent), currentAppWidget(0), waitingForContextMenuObject(0)
{
    setupMenu(editMenu, toolbar);

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(focusWidgetChanged(QWidget*,QWidget*)));
}

AppEditMenu::~AppEditMenu()
{
    WidgetHelper::deleteMenu(editAdvancedMenu);
    WidgetHelper::deleteMenu(editResolveMenu);
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
    //editCopyAsAction=editMenu->addAction(IconUtil::getIcon("editcopy"), tr("Copy as"));
    //editCopyAsAction->setMenu(createCopyAsMenu(editMenu));
    editPasteAction=editMenu->addAction(IconUtil::getIcon("editpaste"), tr("&Paste"), this, SLOT(paste()), QKeySequence(QKeySequence::Paste));
    editPasteAction->setStatusTip(tr("Paste"));
    toolbar->addAction(editPasteAction);

    editMenu->addSeparator();

    editSelectAllAction=editMenu->addAction(IconUtil::getIcon("editselectall"), tr("Select &All"), this, SLOT(selectAll()), QKeySequence(QKeySequence::SelectAll));
    editSelectAllAction->setStatusTip(tr("Select All"));

    editMenu->addSeparator();
    toolbar->addSeparator();

    editAdvancedAction = editMenu->addAction(tr("Advanced"));
    createEditAdvancedMenu(toolbar);
    editAdvancedAction->setMenu(editAdvancedMenu);

    editMenu->addSeparator();

    editDescribeAction=editMenu->addAction(tr("Describe object"), this, SLOT(describeObject()), QKeySequence("F4"));

    editResolveAction=editMenu->addAction(tr("Object Menu"));
    createEditResolveMenu();
    editResolveAction->setMenu(editResolveMenu);

    editMenu->addSeparator();
    //toolbar->addSeparator();

    editIncreaseFontSize=editMenu->addAction(tr("Increase font size"), this, SLOT(increaseFont()), QKeySequence(QKeySequence::ZoomIn));
    editIncreaseFontSize->setStatusTip(tr("Increase font size"));

    editDecreaseFontSize=editMenu->addAction(tr("Decrease font size"), this, SLOT(decreaseFont()), QKeySequence(QKeySequence::ZoomOut));
    editDecreaseFontSize->setStatusTip(tr("Decrease font size"));

    editResetFontSize=editMenu->addAction(tr("Reset font size"), this, SLOT(resetFont()), QKeySequence(tr("Ctrl+0", "Edit|Reset font size")));
    editResetFontSize->setStatusTip(tr("Reset font size"));

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

    editGoToLineAction=editMenu->addAction(IconUtil::getIcon("goto_line"), tr("&Go to line..."), this, SLOT(goToLine()), QKeySequence("Ctrl+G"));
    editGoToLineAction->setStatusTip(tr("Go to line"));


    connect(editResolveMenu, SIGNAL(aboutToShow()), this, SLOT(populateResolveMenu()));
}

QMenu *AppEditMenu::createCopyAsMenu(QWidget *parent)
{
    QMenu *menu=new QMenu(tr("Copy as"), parent);

    menu->addAction(tr("HTML"));
    menu->addAction(tr("C++"));
    menu->addAction(tr("C#"));
    menu->addAction(tr("Java"));

    return menu;
}

QWidget *AppEditMenu::findParentSearchPane() const
{
    return WidgetHelper::findParentWidget(currentAppWidget, "CodeEditorAndSearchPaneWidget");
}

void AppEditMenu::createEditAdvancedMenu(QToolBar *toolbar)
{
    editAdvancedMenu = new QMenu(tr("Advanced"));

    editCommentAction=editAdvancedMenu->addAction(IconUtil::getIcon("comment"), tr("Co&mment/Uncomment"), this, SLOT(comment()), QKeySequence("Ctrl+/"));
    editCommentAction->setStatusTip(tr("Comment/Uncomment line(s)"));
    toolbar->addAction(editCommentAction);

    editMoveUpAction=editAdvancedMenu->addAction(IconUtil::getIcon("move_up"), tr("Move &up"), this, SLOT(moveUp()), QKeySequence("Ctrl+Up"));
    editMoveUpAction->setStatusTip(tr("Move lines(s) up"));
    toolbar->addAction(editMoveUpAction);

    editMoveDownAction=editAdvancedMenu->addAction(IconUtil::getIcon("move_down"), tr("Move &down"), this, SLOT(moveDown()), QKeySequence("Ctrl+Down"));
    editMoveDownAction->setStatusTip(tr("Move lines(s) down"));
    toolbar->addAction(editMoveDownAction);

    editSelectBlockAction=editAdvancedMenu->addAction(tr("Select current block"), this, SLOT(selectBlock()), QKeySequence("Ctrl+B"));
    editSelectBlockAction->setStatusTip(tr("Select current block"));

    editToUpperCaseAction=editAdvancedMenu->addAction(tr("To upper case"), this, SLOT(toUpperCase()), QKeySequence("Ctrl+U"));
    editToUpperCaseAction->setStatusTip(tr("Change selection or current word text to upper case"));

    editToLowerCaseAction=editAdvancedMenu->addAction(tr("To lower case"), this, SLOT(toLowerCase()), QKeySequence("Ctrl+L"));
    editToLowerCaseAction->setStatusTip(tr("Change selection or current word text to lower case"));

    editCreateDuplicateAction=editAdvancedMenu->addAction(tr("Make duplicate"), this, SLOT(makeDuplicate()), QKeySequence("Ctrl+D"));
    editCreateDuplicateAction->setStatusTip(tr("Create duplicate of current line or selection"));

    editRemoveEmptyLinesAction=editAdvancedMenu->addAction(tr("Remove empty lines"), this, SLOT(removeEmptyLines()), QKeySequence("Ctrl+R"));
    editRemoveEmptyLinesAction->setStatusTip(tr("Remove empty lines"));
}

void AppEditMenu::createEditResolveMenu()
{
    editResolveMenu = new QMenu(tr("Object Menu"));
}

void AppEditMenu::updateActionStates(ConnectionPage * /*cnPage*/, ConnectionPageTab * /*cnPageTab*/)
{
    /*bool enable = false;
    if(currentAppWidget!=0 && cnPageTab!=0){
        CodeEditor *editor = qobject_cast<CodeEditor*>(currentAppWidget);
        if(editor!=0){
            enable = (findParentSearchPane()!=0);
        }
    }*/
    bool enable = true;

    editFindAction->setEnabled(enable);
    editFindNextAction->setEnabled(enable);
    editFindPreviousAction->setEnabled(enable);
}

void AppEditMenu::updateActionStatesForCodeEditor(CodeEditor *editor)
{
    Q_ASSERT(editor);
    bool isReadOnly=editor->isReadOnly();

    editUndoAction->setEnabled(!isReadOnly && editor->document()->availableUndoSteps()>0);
    editRedoAction->setEnabled(!isReadOnly && editor->document()->availableRedoSteps()>0);
    editCutAction->setEnabled(!isReadOnly);
    editCopyAction->setEnabled(true);
    //editCopyAsAction->setEnabled(true);
    editPasteAction->setEnabled(!isReadOnly);
    editSelectAllAction->setEnabled(true);

    editCommentAction->setEnabled(!isReadOnly);
    editMoveUpAction->setEnabled(!isReadOnly);
    editMoveDownAction->setEnabled(!isReadOnly);
    editSelectBlockAction->setEnabled(!isReadOnly);
    editToUpperCaseAction->setEnabled(!isReadOnly);
    editToLowerCaseAction->setEnabled(!isReadOnly);
    editCreateDuplicateAction->setEnabled(!isReadOnly);
    editRemoveEmptyLinesAction->setEnabled(!isReadOnly);

    editDescribeAction->setEnabled(true);
    editResolveAction->setEnabled(true);

    editIncreaseFontSize->setEnabled(true);
    editDecreaseFontSize->setEnabled(true);
    editResetFontSize->setEnabled(true);

    editGoToLineAction->setEnabled(true);
}

void AppEditMenu::setUndoEnabled(bool enabled)
{
    editUndoAction->setEnabled(enabled);
}

void AppEditMenu::setRedoEnabled(bool enabled)
{
    editRedoAction->setEnabled(enabled);
}

void AppEditMenu::focusWidgetChanged(QWidget * /*old*/, QWidget *now)
{
    if(now==0){
        return;
    }

    currentAppWidget=now;

    ConnectionPage *cnPage=getConnectionsPane()->currentConnectionPage();
    ConnectionPageTab *cnPageTab=cnPage ? cnPage->currentConnectionPageTab() : 0;
    updateActionStates(cnPage, cnPageTab);

    CodeEditor *editor = qobject_cast<CodeEditor*>(currentAppWidget);
    bool isCodeEditor=(editor!=0);
    if(isCodeEditor){
        updateActionStatesForCodeEditor(editor);
        return;
    }

    bool canUndo=now->metaObject()->indexOfSlot("undo()")>-1;
    bool canRedo=now->metaObject()->indexOfSlot("redo()")>-1;

    editUndoAction->setEnabled(canUndo);
    editRedoAction->setEnabled(canRedo);

    bool canCut=now->metaObject()->indexOfSlot("cut()")>-1;
    bool canCopy=now->metaObject()->indexOfSlot("copy()")>-1;
    bool canPaste=now->metaObject()->indexOfSlot("paste()")>-1;
    bool canSelectAll=now->metaObject()->indexOfSlot("selectAll()")>-1;

    editCutAction->setEnabled(canCut);
    editCopyAction->setEnabled(canCopy);
    //editCopyAsAction->setEnabled(false);
    editPasteAction->setEnabled(canPaste);
    editSelectAllAction->setEnabled(canSelectAll);

    editCommentAction->setEnabled(false);
    editMoveUpAction->setEnabled(false);
    editMoveDownAction->setEnabled(false);
    editSelectBlockAction->setEnabled(false);
    editToUpperCaseAction->setEnabled(false);
    editToLowerCaseAction->setEnabled(false);
    editCreateDuplicateAction->setEnabled(false);
    editRemoveEmptyLinesAction->setEnabled(false);

    editDescribeAction->setEnabled(false);
    editResolveAction->setEnabled(false);

    editIncreaseFontSize->setEnabled(false);
    editDecreaseFontSize->setEnabled(false);
    editResetFontSize->setEnabled(false);

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
    if(qobject_cast<CodeEditor*>(currentAppWidget)){
        WidgetHelper::invokeSlot(currentAppWidget, "customCut");
    }else{
        WidgetHelper::invokeSlot(currentAppWidget, "cut");
    }
}

void AppEditMenu::copy()
{
    if(qobject_cast<CodeEditor*>(currentAppWidget)){
        WidgetHelper::invokeSlot(currentAppWidget, "customCopy");
    }else{
        WidgetHelper::invokeSlot(currentAppWidget, "copy");
    }
}

void AppEditMenu::paste()
{
    WidgetHelper::invokeSlot(currentAppWidget, "paste");
}

void AppEditMenu::selectAll()
{
    WidgetHelper::invokeSlot(currentAppWidget, "selectAll");
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

void AppEditMenu::resetFont()
{
    WidgetHelper::invokeSlot(currentAppWidget, "resetFontSize");
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

void AppEditMenu::removeEmptyLines()
{
    WidgetHelper::invokeSlot(currentAppWidget, "removeEmptyLines");
}

void AppEditMenu::describeObject()
{
    CodeEditor *editor = qobject_cast<CodeEditor*>(currentAppWidget);
    if(!editor){
        return;
    }
    QString currentObjectName = CodeEditorUtil::getCurrentObjectName(editor);
    if(currentObjectName.isEmpty()){
        QMessageBox::critical(MainWindow::defaultInstance(), tr("No selection"),
                              tr("Please, select object name and try again."));
        return;
    }

    if(currentObjectName.length()>250){
        QMessageBox::critical(MainWindow::defaultInstance(), tr("Selection too long"),
                              tr("Selection is too long. Please, select object name and try again."));
        return;
    }

    uiManager()->describeObject(currentObjectName);
}

void AppEditMenu::populateResolveMenu()
{
    CodeEditor *editor = qobject_cast<CodeEditor*>(currentAppWidget);
    if(!editor){
        return;
    }

    WidgetHelper::deleteActions(editResolveMenu->actions());

    QString currentObjectName = CodeEditorUtil::getCurrentObjectName(editor);

    qDebug() << "currentObjectName =" << currentObjectName;

    bool willResolve = !currentObjectName.isEmpty() && currentObjectName.length() < 250;

    QAction *statusAction = editResolveMenu->addAction(willResolve ? tr("Resolving...") : tr("No object selected"));
    statusAction->setEnabled(false);

    if(willResolve){
        waitingForContextMenuObject = currentAppWidget;

        IQueryScheduler *queryScheduler = currentConnectionPageTab();
        ObjectResolver *resolver = new ObjectResolver(this);
        connect(resolver, SIGNAL(resolved(QString,DbTreeModel::DbTreeNodeType,QString,QString,ObjectResolver*)), this, SLOT(objectResolved(QString,DbTreeModel::DbTreeNodeType,QString,QString,ObjectResolver*)));
        connect(resolver, SIGNAL(resolveError(OciException,ObjectResolver*)), this, SLOT(objectResolveError(OciException,ObjectResolver*)));
        resolver->resolveName(currentObjectName, queryScheduler);
    }
}

void AppEditMenu::objectResolved(const QString &objectTypeName, DbTreeModel::DbTreeNodeType objectType, const QString &schemaName, const QString &objectName, ObjectResolver *resolver)
{
    Q_UNUSED(objectTypeName);

    resolver->deleteLater();

    if(waitingForContextMenuObject != currentAppWidget){
        return;
    }

    QList<QAction*> objectActions = ContextMenuUtil::getActionsForObject(schemaName, objectName, "", objectType, uiManager());

    WidgetHelper::deleteActions(editResolveMenu->actions());

    editResolveMenu->setUpdatesEnabled(false);
    editResolveMenu->addActions(objectActions);
    editResolveMenu->setUpdatesEnabled(true);
}

void AppEditMenu::objectResolveError(const OciException &ex, ObjectResolver *resolver)
{
    resolver->deleteLater();

    qDebug() << "objectResolveError" << ex.getErrorMessage();

    if(ex.getErrorCode() == ERR_OBJECT_DOES_NOT_EXIST){
        editResolveMenu->actions().at(0)->setText(tr("Object does not exist"));
    }else{
        editResolveMenu->actions().at(0)->setText(tr("Error resolving object"));
        qDebug() << "resolveQueryCompleted" << ex.getErrorMessage();
    }
}

void AppEditMenu::showSearchWidget()
{
    QWidget *w = findParentSearchPane();
    if(w){
        WidgetHelper::invokeSlot(w, "showSearchPane");
    }
}

void AppEditMenu::findNext()
{
    QWidget *w = findParentSearchPane();
    if(w){
        WidgetHelper::invokeSlot(w, "findNext");
    }
}

void AppEditMenu::findPrevious()
{
    QWidget *w = findParentSearchPane();
    if(w){
        WidgetHelper::invokeSlot(w, "findPrevious");
    }
}

void AppEditMenu::goToLine()
{
    WidgetHelper::invokeSlot(currentAppWidget, "goToLine");
}

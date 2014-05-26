#include "codestructurepane.h"
#include "codestructuretreeview.h"
#include "codestructuremodel.h"
#include "code_parser/plsql/plsqltreebuilder.h"
#include "code_parser/plsql/plsqlrules.h"
#include <QtGui>

CodeStructurePane::CodeStructurePane(QWidget *parent) :
    QWidget(parent),
    treeBuilder(0),
    isActive(true)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(2,0,0,0);
    mainLayout->setSpacing(5);

    treeView = new CodeStructureTreeView();
    mainLayout->addWidget(treeView);

    setLayout(mainLayout);
}

QSize CodeStructurePane::sizeHint() const
{
    return QSize(200, 200);
}

void CodeStructurePane::setCurrentWidget(MultiEditorWidget *editor)
{
    if(this->currentEditor == editor){
        return;
    }
    this->treeBuilder = 0;
    this->currentEditor = editor;

    setNewModel(); //will delete current model because treeBuilder is 0
}

void CodeStructurePane::unregisterWidget(MultiEditorWidget *editor)
{
    if(this->currentEditor == editor){
        this->currentEditor = 0;
        this->treeBuilder = 0;

        setNewModel();
    }
}

void CodeStructurePane::setTreeBuilder(MultiEditorWidget *editor, PlSqlTreeBuilder *treeBuilder, int cursorPosition)
{
    if(this->currentEditor != editor){
        return;
    }

    if(this->treeBuilder == treeBuilder){
        return;
    }

    this->treeBuilder = treeBuilder;

    setNewModel();
    setCursorPosition(editor, cursorPosition);
}

PlSqlTreeBuilder *CodeStructurePane::currentTreeBuilder() const
{
    return this->treeBuilder;
}

void CodeStructurePane::setCursorPosition(MultiEditorWidget *editor, int position)
{
    if(this->currentEditor != editor){
        return;
    }

    CodeStructureModel *model = static_cast<CodeStructureModel*>(treeView->model());
    if(model){
        QModelIndex highlightedIndex = treeView->findFirstVisibleParent(model->setCursorPosition(position));
        if(highlightedIndex.isValid()){
            treeView->scrollTo(highlightedIndex);
        }
    }
}

void CodeStructurePane::setActive(bool active)
{
    this->isActive = active;

    if(active && currentEditor != 0 && treeBuilder != 0 && treeView->model()==0){
        setNewModel();
        setCursorPosition(currentEditor, currentEditor->currentTextEditor()->textCursor().position());
    }
}

void CodeStructurePane::setNewModel()
{
    QAbstractItemModel *oldModel = treeView->model();
    QItemSelectionModel *oldSelModel = treeView->selectionModel();

    CodeStructureModel *newModel = ((isActive && treeBuilder) ? new CodeStructureModel(treeBuilder->getRootNode(), this) : 0);
    treeView->setModel(newModel);

    if(newModel){
        treeView->header()->setResizeMode(QHeaderView::ResizeToContents);
        treeView->header()->setStretchLastSection(false);
    }

    delete oldModel;
    delete oldSelModel;
}

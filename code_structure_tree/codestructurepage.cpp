#include "codestructurepage.h"
#include "codestructuretreeview.h"
#include "codestructuremodel.h"
#include "codestructuretreeitem.h"
#include "code_parser/plsql/plsqltreebuilder.h"
#include "code_parser/plsql/plsqlrules.h"
#include "widgets/multieditorwidget.h"
#include "widgets/toolbar.h"
#include "codestructuresortfiltermodel.h"
#include "util/iconutil.h"
#include <QtGui>

CodeStructurePage::CodeStructurePage(MultiEditorWidget *editor, QWidget *parent) :
    QWidget(parent),
    editor(editor)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    ToolBar *toolbar = new ToolBar();
    sortAction = toolbar->addAction(IconUtil::getIcon("sort"), "Sort");
    sortAction->setCheckable(true);
    sortAction->setChecked(false);
    mainLayout->addWidget(toolbar);

    treeView = new CodeStructureTreeView();
    sortModel = new CodeStructureSortFilterModel(this);
    treeView->setModel(sortModel);
    treeView->header()->setResizeMode(QHeaderView::ResizeToContents);
    treeView->header()->setStretchLastSection(false);
    mainLayout->addWidget(treeView);

    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    connect(sortAction, SIGNAL(triggered(bool)), this, SLOT(sortButtonClicked(bool)));
    connect(editor, SIGNAL(codeStructureModelAvailable(CodeStructureModel*)), this, SLOT(codeStructureModelAvailable(CodeStructureModel*)));
    connect(treeView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(selectionChanged(QItemSelection,QItemSelection)));
}

void CodeStructurePage::setCursorPosition(const QModelIndex &index)
{
    //CodeStructureModel *model = static_cast<CodeStructureModel*>(sortModel->sourceModel());
    if(sortModel->sourceModel()){
        QModelIndex highlightedIndex = treeView->findFirstVisibleParent(sortModel->mapFromSource(index));
        if(highlightedIndex.isValid()){
            treeView->scrollTo(highlightedIndex);
        }
    }
}

void CodeStructurePage::sortButtonClicked(bool sort)
{
    sortModel->sort(sort ? 0 : -1);
}

void CodeStructurePage::codeStructureModelAvailable(CodeStructureModel *model)
{
    setNewModel(model);
    connect(model, SIGNAL(cursorPositionChanged(QModelIndex)), this, SLOT(cursorPositionChanged(QModelIndex)));
}

void CodeStructurePage::cursorPositionChanged(const QModelIndex &index)
{
    setCursorPosition(index);
}

void CodeStructurePage::setNewModel(CodeStructureModel *newModel)
{
    delete sortModel->sourceModel();
    sortModel->setSourceModel(newModel);
}

void CodeStructurePage::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    if(!selected.isEmpty())   {
        treeCurrentRowChanged(selected.indexes().at(0), QModelIndex());
    }
}

void CodeStructurePage::treeCurrentRowChanged(const QModelIndex & current, const QModelIndex & previous)
{
    //if(!previous.isValid()){
    //    return;
    //}
    Q_UNUSED(previous);

    if(!current.isValid()){
        return;
    }

    if(!sortModel->sourceModel()){
        return;
    }

    QModelIndex sourceIx = sortModel->mapToSource(current);

    CodeStructureTreeItem *item = static_cast<CodeStructureTreeItem*>(sourceIx.internalPointer());
    TokenInfo *tokenInfo = item->getNode()->tokenInfo;
    editor->pulsate(tokenInfo->startPos, tokenInfo->endPos);
}

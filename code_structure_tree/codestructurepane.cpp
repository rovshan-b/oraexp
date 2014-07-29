#include "codestructurepane.h"
#include "codestructuretreeview.h"
#include "codestructuremodel.h"
#include "codestructuretreeitem.h"
#include "code_parser/plsql/plsqltreebuilder.h"
#include "code_parser/plsql/plsqlrules.h"
#include "widgets/toolbar.h"
#include "util/iconutil.h"
#include <QtGui>

CodeStructurePane::CodeStructurePane(QWidget *parent) :
    QWidget(parent),
    currentEditor(0),
    treeBuilder(0),
    isActive(true)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(2,0,0,0);
    mainLayout->setSpacing(5);

    tab = new QStackedWidget();

    createTreePane();
    createLabelPane();

    mainLayout->addWidget(tab);

    setLayout(mainLayout);

    showPane(LabelPane);
}

void CodeStructurePane::createTreePane()
{
    QVBoxLayout *treeLayout = new QVBoxLayout();
    treeLayout->setContentsMargins(0,0,0,0);

    ToolBar *toolbar = new ToolBar();
    sortAction = toolbar->addAction(IconUtil::getIcon("sort"), "Sort");
    sortAction->setCheckable(true);
    sortAction->setChecked(false);
    treeLayout->addWidget(toolbar);

    treeView = new CodeStructureTreeView();
    treeLayout->addWidget(treeView);

    connect(sortAction, SIGNAL(triggered(bool)), this, SLOT(sortButtonClicked(bool)));

    QWidget *w = new QWidget();
    w->setLayout(treeLayout);

    tab->addWidget(w);
}

void CodeStructurePane::createLabelPane()
{
    QLabel *label = new QLabel(tr("No active PL/SQL editor"));

    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);

    tab->addWidget(label);
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

    sortAction->setChecked(sortedEditors.value(this->currentEditor, false));
    sortAction->setEnabled(this->currentEditor != 0);

    setNewModel(); //will delete current model because treeBuilder is 0
    showPane(TreePane);
}

void CodeStructurePane::unregisterWidget(MultiEditorWidget *editor)
{
    if(this->currentEditor == editor){
        this->currentEditor = 0;
        this->treeBuilder = 0;

        sortedEditors.remove(editor);
        sortAction->setEnabled(false);

        setNewModel();

        showPane(LabelPane);
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

    CodeStructureModel *newModel = ((isActive && treeBuilder) ? new CodeStructureModel(treeBuilder->getRootNode(), sortedEditors.value(currentEditor, false), this) : 0);
    treeView->setModel(newModel);

    if(newModel){
        treeView->header()->setResizeMode(QHeaderView::ResizeToContents);
        treeView->header()->setStretchLastSection(false);

        treeView->selectionModel()->setCurrentIndex(newModel->index(0,0), QItemSelectionModel::NoUpdate);
        connect(treeView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(treeCurrentRowChanged(QModelIndex,QModelIndex)));
    }

    delete oldModel;
    delete oldSelModel;
}

void CodeStructurePane::treeCurrentRowChanged(const QModelIndex & current, const QModelIndex & previous)
{
    if(!previous.isValid()){
        return;
    }

    if(!current.isValid()){
        return;
    }

    if(!currentEditor || !treeBuilder){
        return;
    }

    CodeStructureTreeItem *item = static_cast<CodeStructureTreeItem*>(current.internalPointer());
    TokenInfo *tokenInfo = item->getNode()->tokenInfo;
    currentEditor->pulsate(tokenInfo->startPos, tokenInfo->endPos);
}

void CodeStructurePane::sortButtonClicked(bool sort)
{
    if(!currentEditor || !treeBuilder){
        return;
    }

    sortedEditors[currentEditor] = sort;
    setNewModel();
}

void CodeStructurePane::showPane(CodeStructurePane::Pane pane)
{
    tab->setCurrentIndex((int)pane);
}

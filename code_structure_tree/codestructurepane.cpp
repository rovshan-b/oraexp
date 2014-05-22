#include "codestructurepane.h"
#include "codestructuretreeview.h"
#include "codestructuremodel.h"
#include <QtGui>

CodeStructurePane::CodeStructurePane(QWidget *parent) :
    QWidget(parent),
    treeBuilder(0)
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
    this->currentEditor = editor;
}

void CodeStructurePane::setTreeBuilder(MultiEditorWidget *editor, PlSqlTreeBuilder *treeBuilder)
{
    if(this->currentEditor != editor){
        return;
    }

    if(this->treeBuilder == treeBuilder){
        return;
    }

    this->treeBuilder = treeBuilder;

    QAbstractItemModel *oldModel = treeView->model();
    QItemSelectionModel *oldSelModel = treeView->selectionModel();

    treeView->setModel(new CodeStructureModel(treeBuilder, this));

    delete oldModel;
    delete oldSelModel;
}

PlSqlTreeBuilder *CodeStructurePane::currentTreeBuilder() const
{
    return this->treeBuilder;
}

#include "codestructurecombobox.h"
#include "code_structure_tree/codestructuremodel.h"
#include "code_structure_tree/codestructuretreeitem.h"
#include "defines.h"
#include "util/iconutil.h"
#include <QtGui>

CodeStructureComboBox::CodeStructureComboBox(QWidget *parent) :
    QComboBox(parent)
{
    //setMinimumContentsLength(MAX_IDENTIFIER_LENGTH);
    //setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
    setInsertPolicy(QComboBox::NoInsert);

    connect(this, SIGNAL(activated(int)), this, SLOT(indexChanged(int)));
}

void CodeStructureComboBox::codeStructureModelAvailable(CodeStructureModel *model)
{
    setModel(model);

    connect(model, SIGNAL(cursorPositionChanged(QModelIndex)), this, SLOT(cursorPositionChanged(QModelIndex)));
}

void CodeStructureComboBox::cursorPositionChanged(const QModelIndex &index)
{
    QModelIndex parentIndex = index;

    while(parentIndex.parent().isValid()){
        parentIndex = parentIndex.parent();
    }

    setCurrentIndex(parentIndex.row());
}

void CodeStructureComboBox::indexChanged(int index)
{
    CodeStructureModel *currentModel = static_cast<CodeStructureModel*>(model());
    if(!currentModel){
        return;
    }

    CodeStructureTreeItem *item = currentModel->itemForRow(index);
    if(!item){
        return;
    }

    int startPos = item->getTokenInfo()->startPos;
    int endPos = item->getTokenInfo()->endPos;

    emit selectionChanged(endPos, startPos); //make bottom up selection to put cursor at the beginning
}
/*
void CodeStructureComboBox::focusInEvent(QFocusEvent *)
{
    setEditable(true);
    lineEdit()->selectAll();
}

void CodeStructureComboBox::focusOutEvent(QFocusEvent *)
{
    setEditable(false);
}*/

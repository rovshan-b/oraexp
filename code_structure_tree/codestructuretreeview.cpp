#include "codestructuretreeview.h"

CodeStructureTreeView::CodeStructureTreeView(QWidget *parent) :
    QTreeView(parent)
{
    setHeaderHidden(true);
    setUniformRowHeights(true);
    setRootIsDecorated(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
}

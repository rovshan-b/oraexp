#include "codestructuretreeview.h"

CodeStructureTreeView::CodeStructureTreeView(QWidget *parent) :
    QTreeView(parent)
{
    setHeaderHidden(true);
    setUniformRowHeights(true);
    setRootIsDecorated(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
}

QModelIndex CodeStructureTreeView::findFirstVisibleParent(const QModelIndex &child) const
{
    QModelIndex ix = child;
    while(ix.isValid()){

        if(!ix.parent().isValid() || isExpanded(ix.parent())){
            return ix;
        }

        ix = ix.parent();
    }

    return QModelIndex();
}

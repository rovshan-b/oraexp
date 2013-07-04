#include "treeview.h"
#include <QtGui>

TreeView::TreeView(QWidget *parent) :
    QTreeView(parent)
{
}

void TreeView::resizeColumnsToContents()
{
    for(int i=0; i<header()->count(); ++i){
        //int size = qMax(sizeHintForColumn(i), header()->sizeHintForColumn(i)) + 15;
        //size = qMin(size, 300);

        int size = qMin(sizeHintForColumn(i) + 15, 300);
        header()->resizeSection(i, size);
    }
}

void TreeView::drawRow(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QTreeView::drawRow(painter, option, index);

    if(!index.isValid()){
        return;
    }

    for (int col = 0; col < header()->count(); ++col)
    {
        QModelIndex s = index.sibling(index.row(), col);
        if (s.isValid())
        {
            QRect rect = visualRect(s);
            QPen pen;
            pen.setWidthF(0.5);
            pen.setColor(Qt::gray);
            painter->setPen(pen);
            painter->drawRect(rect);
        }
    }
}

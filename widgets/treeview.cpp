#include "treeview.h"
#include "widgets/treeviewverticalheader.h"
#include <QtGui>

TreeView::TreeView(QWidget *parent) :
    QTreeView(parent)
{
    verticalHeader = new TreeViewVerticalHeader(this);
    connect(verticalScrollBar(), SIGNAL(valueChanged(int)), verticalHeader, SLOT(update()));
    connect(this, SIGNAL(expanded(QModelIndex)), verticalHeader, SLOT(update()));
    connect(this, SIGNAL(collapsed(QModelIndex)), verticalHeader, SLOT(update()));

    setUniformRowHeights(true);
    //setAlternatingRowColors(true);
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

/*
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

            if(!rect.isValid()){
                continue;
            }

            QPen pen;
            pen.setWidthF(0.5);
            pen.setColor(Qt::gray);
            painter->setPen(pen);
            painter->drawRect(rect);

//            if(s.column() == 0){
//                //painter->fillRect(0,rect.top(),10,rect.height(), Qt::green);
//                //style()->drawControl(QStyle::CE_Header, &option, painter, this);
//                QStyleOptionHeader opt;
//                opt.initFrom(this);
//                opt.orientation = Qt::Vertical;
//                opt.text = "1";
//                opt.textAlignment = Qt::AlignRight;
//                opt.rect = QRect(0,rect.top(),20,rect.height());
//                style()->drawControl(QStyle::CE_HeaderSection, &opt, painter, this);
//            }
        }
    }
}*/

void TreeView::drawVerticalHeaderCells(QStyleOptionHeader &opt, QPainter *painter, const QModelIndex &parent, QString &path, bool *widthChanged)
{
    int level = 0;
    for(int i=0; i<model()->rowCount(parent); ++i){

        QString text = path;
        if(!text.isEmpty()){
            text.append(".");
        }
        text.append(QString::number(++level));
        opt.text = text;

        if(text.length() > maxVertHeaderText.length()){
            maxVertHeaderText = text;
            *widthChanged = true;
        }

        QModelIndex childIndex = model()->index(i, 0, parent);
        drawVerticalHeaderCell(opt, painter, childIndex);

        if(model()->hasChildren(childIndex)){
            drawVerticalHeaderCells(opt, painter, childIndex, text, widthChanged);
        }
    }
    --level;
}

void TreeView::drawVerticalHeaderCell(QStyleOptionHeader &opt, QPainter *painter, const QModelIndex &index)
{
    QRect indexRect = visualRect(index);
    if(!indexRect.isValid()){
        return;
    }
    opt.rect = QRect(0, indexRect.top(), verticalHeaderWidth, indexRect.height());
    style()->drawControl(QStyle::CE_HeaderSection, &opt, painter, verticalHeader);
    opt.rect.adjust(2,0,0,0);
    style()->drawControl(QStyle::CE_HeaderLabel, &opt, painter, verticalHeader);
}

void TreeView::verticalHeaderPaintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(verticalHeader);

    QStyleOptionHeader opt;
    opt.initFrom(this);
    opt.orientation = Qt::Vertical;
    opt.textAlignment = Qt::AlignLeft;
    opt.state = QStyle::State_None;
    opt.position = QStyleOptionHeader::Middle;

    QString text;
    bool widthChanged = false;
    drawVerticalHeaderCells(opt, &painter, QModelIndex(), text, &widthChanged);

    if(widthChanged){
        updateVerticalHeaderWidth();
    }
}

void TreeView::resizeEvent(QResizeEvent *event)
{
    QTreeView::resizeEvent(event);

    updateVerticalHeaderWidth();
}

void TreeView::updateVerticalHeaderWidth()
{
    verticalHeaderWidth = fontMetrics().width(maxVertHeaderText) + 5;

    //if(newWidth == verticalHeaderWidth){
    //    verticalHeader->update();
    //    return;
    //}

    int top = header()->height()+1;

    setViewportMargins(verticalHeaderWidth, top, 0, 0);
    QRect cr = contentsRect();
    verticalHeader->setGeometry(cr.left(),top,verticalHeaderWidth,cr.height()-top);
}

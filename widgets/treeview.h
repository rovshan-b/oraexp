#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>

class TreeViewVerticalHeader;

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TreeView(QWidget *parent = 0);
    
    void resizeColumnsToContents();

    void verticalHeaderPaintEvent(QPaintEvent *event);

protected:
    void drawRow ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    void drawVerticalHeaderCells(QStyleOptionHeader &opt, QPainter *painter, const QModelIndex &parent, QString &path);
    void drawVerticalHeaderCell(QStyleOptionHeader &opt, QPainter *painter, const QModelIndex &index);

    void resizeEvent(QResizeEvent *event);

private:
    TreeViewVerticalHeader *verticalHeader;

    void updateVerticalHeader();
    int verticalHeaderWidth() const;
};

#endif // TREEVIEW_H

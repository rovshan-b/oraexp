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

    void verticalHeaderPaintEvent(QPaintEvent *event = 0);

protected:
    //void drawRow ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;

    void drawVerticalHeaderCells(QStyleOptionHeader &opt, QPainter *painter, const QModelIndex &parent, QString &path, bool *widthChanged);
    void drawVerticalHeaderCell(QStyleOptionHeader &opt, QPainter *painter, const QModelIndex &index);

    void resizeEvent(QResizeEvent *event);

private slots:
    void updateVerticalHeaderWidth();

private:
    TreeViewVerticalHeader *verticalHeader;
    QString maxVertHeaderText;
    int verticalHeaderWidth;
};

#endif // TREEVIEW_H

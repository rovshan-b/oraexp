#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TreeView(QWidget *parent = 0);
    
    void resizeColumnsToContents();

protected:
    void drawRow ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    
};

#endif // TREEVIEW_H

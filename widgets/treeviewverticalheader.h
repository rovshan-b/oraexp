#ifndef TREEVIEWVERTICALHEADER_H
#define TREEVIEWVERTICALHEADER_H

#include <QWidget>

#include "widgets/treeview.h"

class TreeViewVerticalHeader : public QWidget
{
    Q_OBJECT
public:
    explicit TreeViewVerticalHeader(TreeView *parent);
    
protected:
    void paintEvent(QPaintEvent *event) {
        static_cast<TreeView*>(parentWidget())->verticalHeaderPaintEvent(event);
    }
    
};

#endif // TREEVIEWVERTICALHEADER_H

#ifndef CODESTRUCTUREWIDGET_H
#define CODESTRUCTUREWIDGET_H

#include <QTreeView>

class CodeStructureWidget : public QTreeView
{
    Q_OBJECT
public:
    explicit CodeStructureWidget(QWidget *parent = 0);
    
};

#endif // CODESTRUCTUREWIDGET_H

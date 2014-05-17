#ifndef CODESTRUCTUREPANE_H
#define CODESTRUCTUREPANE_H

#include <QWidget>

class CodeStructureWidget;

class CodeStructurePane : public QWidget
{
    Q_OBJECT
public:
    explicit CodeStructurePane(QWidget *parent = 0);
    
    virtual QSize sizeHint () const;

private:
    CodeStructureWidget *structureWidget;
    
};

#endif // CODESTRUCTUREPANE_H

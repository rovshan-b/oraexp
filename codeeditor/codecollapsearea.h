#ifndef CODECOLLAPSEAREA_H
#define CODECOLLAPSEAREA_H

#include <QWidget>

#include "codeeditor.h"

class CodeCollapseArea : public QWidget
{
    Q_OBJECT
public:
    explicit CodeCollapseArea(CodeEditor *codeEditor);
    
    QSize sizeHint() const {
        return QSize(codeEditor->codeCollapseAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->codeCollapseAreaPaintEvent(event);
    }
    virtual void wheelEvent(QWheelEvent * event){
        codeEditor->lineNavBarWheelEvent(event);
    }

private:
    CodeEditor *codeEditor;
    
};

#endif // CODECOLLAPSEAREA_H

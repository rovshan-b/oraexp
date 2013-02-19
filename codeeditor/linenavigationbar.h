#ifndef LINENAVIGATIONBAR_H
#define LINENAVIGATIONBAR_H

#include <QWidget>
#include "codeeditor.h"

class LineNavigationBar : public QWidget
{
    Q_OBJECT
public:
    explicit LineNavigationBar(CodeEditor *codeEditor);
    
    QSize sizeHint() const {
        return QSize(16, 0);
    }

protected:
    virtual void paintEvent(QPaintEvent *event) {
        codeEditor->lineNavBarPaintEvent(event);
    }
    virtual void mouseReleaseEvent(QMouseEvent * event){
        codeEditor->lineNavBarMouseReleaseEvent(event);
    }
    virtual void wheelEvent(QWheelEvent * event){
        codeEditor->lineNavBarWheelEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif // LINENAVIGATIONBAR_H

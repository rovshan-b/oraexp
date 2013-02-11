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

    //line number, colors
    QHash<int, QList<QColor> > highlightColors;

protected:
    virtual void paintEvent(QPaintEvent *event) {
        codeEditor->lineNavBarPaintEvent(event);
    }
    virtual void mouseReleaseEvent (QMouseEvent * event){
        codeEditor->lineNavBarMouseReleaseEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif // LINENAVIGATIONBAR_H

#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include "codeeditor.h"

class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumberArea(CodeEditor *codeEditor);

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }
    virtual void wheelEvent(QWheelEvent * event){
        codeEditor->lineNavBarWheelEvent(event);
    }

private:
    CodeEditor *codeEditor;

};

#endif // LINENUMBERAREA_H

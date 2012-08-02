#include "linenumberarea.h"
#include "codeeditor.h"

LineNumberArea::LineNumberArea(CodeEditor *codeEditor) :
    QWidget(codeEditor)
{
    this->codeEditor=codeEditor;
}

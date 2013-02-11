#include "linenavigationbar.h"
#include <QtGui>

LineNavigationBar::LineNavigationBar(CodeEditor *codeEditor) :
    QWidget(codeEditor)
{
    this->codeEditor=codeEditor;
    this->setFixedWidth(16);
    this->setCursor(Qt::PointingHandCursor);
}

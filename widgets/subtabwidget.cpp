#include "subtabwidget.h"
#include <QTabBar>

SubTabWidget::SubTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    setDocumentMode(true);
    QFont newFont;
    qreal newFontSize=this->tabBar()->font().pointSizeF()-1;
    newFont.setPointSizeF(newFontSize);
    this->tabBar()->setFont(newFont);
}

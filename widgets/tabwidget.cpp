#include "tabwidget.h"
#include <QtGui>

TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent)
{
}

void TabWidget::setTabBusy(int tabIx, bool busy)
{
    tabBar()->setTabTextColor(tabIx, busy ? QColor(Qt::blue) : tabBar()->palette().windowText().color());
}

void TabWidget::setTabBusy(QWidget *tab, bool busy)
{
    setTabBusy(indexOf(tab), busy);
}

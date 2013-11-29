#include "connectionpagetabwidget.h"
//#include "tabbar.h"

ConnectionPageTabWidget::ConnectionPageTabWidget(QWidget *parent) :
    TabWidget(parent)
{
    //TabBar *b = new TabBar();
    //setTabBar(b);

    //connect(b, SIGNAL(setCornerWidget(QWidget*)), this, SLOT(cornerWidgetRequested(QWidget*)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
}

int ConnectionPageTabWidget::insertTab(int index, QWidget *page, const QIcon &icon, const QString &label)
{
    tabChangeHistory.append(page);

    return TabWidget::insertTab(index, page, icon, label);
}

void ConnectionPageTabWidget::removeTab(int index)
{
    tabChangeHistory.removeOne(widget(index));

    TabWidget::removeTab(index);
}

void ConnectionPageTabWidget::currentTabChanged(int index)
{
    if(index==-1){
        return;
    }

    QWidget *w = widget(index);

    Q_ASSERT(tabChangeHistory.contains(w));

    tabChangeHistory.removeOne(w);
    tabChangeHistory.prepend(w);
}

/*
void ConnectionPageTabWidget::cornerWidgetRequested(QWidget *w)
{
    setCornerWidget(w, Qt::TopRightCorner);
}*/

#include "connectionpagetabwidget.h"

ConnectionPageTabWidget::ConnectionPageTabWidget(QWidget *parent) :
    TabWidget(parent)
{
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

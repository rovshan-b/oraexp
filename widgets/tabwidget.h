#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = 0);

    QTabBar *bar() const {return this->tabBar();}

    void setTabBusy(int tabIx, bool busy);
    void setTabBusy(QWidget *tab, bool busy);

signals:

public slots:

};

#endif // TABWIDGET_H

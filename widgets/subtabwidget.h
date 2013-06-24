#ifndef SUBTABWIDGET_H
#define SUBTABWIDGET_H

#include <QTabWidget>
#include <QTabBar>

class SubTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit SubTabWidget(QWidget *parent = 0);

    void hideTabBar(){tabBar()->hide();}
    void setTabBarVisible(bool visible){tabBar()->setVisible(visible);}

signals:

public slots:

};

#endif // SUBTABWIDGET_H

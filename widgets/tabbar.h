#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>

class QToolButton;

class TabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit TabBar(QWidget *parent = 0);

    virtual QSize sizeHint() const;

signals:
    void setCornerWidget(QWidget *w);

protected:
    virtual void resizeEvent ( QResizeEvent * event );
    virtual void tabLayoutChange ();
    
private:
    QToolButton *addButton;
    bool addButtonVisible;

    void movePlusButton();
    
};

#endif // TABBAR_H

#ifndef CONNECTIONPAGETABWIDGET_H
#define CONNECTIONPAGETABWIDGET_H

#include "tabwidget.h"
#include <QList>

class ConnectionPageTabWidget : public TabWidget
{
    Q_OBJECT
public:
    explicit ConnectionPageTabWidget(QWidget *parent = 0);

    int insertTab ( int index, QWidget * page, const QIcon & icon, const QString & label);
    void removeTab(int index);

    QList<QWidget*> getTabChangeHistory() const {return this->tabChangeHistory;}
    
private slots:
    void currentTabChanged(int index);

private:
    QList<QWidget*> tabChangeHistory;
    
};

#endif // CONNECTIONPAGETABWIDGET_H

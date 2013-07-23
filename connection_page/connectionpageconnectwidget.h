#ifndef CONNECTIONPAGECONNECTWIDGET_H
#define CONNECTIONPAGECONNECTWIDGET_H

#include <QWidget>

class QSplitter;

class ConnectionPageConnectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionPageConnectWidget(QWidget *parent = 0);
    
private:
    void createUi();

    void createConnectionList(QSplitter *splitter);
    void createConnectionEditor(QSplitter *splitter);
    
};

#endif // CONNECTIONPAGECONNECTWIDGET_H

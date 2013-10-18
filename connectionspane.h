#ifndef CONNECTIONSPANE_H
#define CONNECTIONSPANE_H

#include "widgets/tabwidget.h"

class DbConnection;
class ConnectionPage;
class DbConnectionInfo;

class ConnectionsPane : public TabWidget
{
    Q_OBJECT
public:
    explicit ConnectionsPane(QWidget *parent = 0);

    void addConnection(const QString &uuid = QString());
    ConnectionPage *currentConnectionPage() const;

signals:
    void connectionPaneStateChanged();
    void connectionPageStateChanged();

    void canClose();

public slots:
    void closeAll();
    void closeTab(int index);
    void popOutTab();
    void ctrlTabPressed();
    void setTabTitle(QWidget *tab, DbConnectionInfo *connectionInfo);

    void tabBusyStateChanged(ConnectionPage *cnPage, bool busy);

protected:
    void contextMenuEvent(QContextMenuEvent* pEvent);

private:
    void showTabBar();
    void updateMainWindowTitle();

};

#endif // CONNECTIONSPANE_H

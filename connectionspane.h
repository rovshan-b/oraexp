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

    bool isBusy() const;

signals:
    void connectionPaneStateChanged();
    void connectionPageStateChanged();

    void canExit();

public slots:
    bool closeAndExit();
    void closeTab(int index);
    void popOutTab();
    void ctrlTabPressed();
    void setTabTitle(QWidget *tab, DbConnectionInfo *connectionInfo);

    void tabBusyStateChanged(ConnectionPage *cnPage, bool busy);

private slots:
    void checkConnectionCountAndExit();
    void disconnected(DbConnection *db);

protected:
    void contextMenuEvent(QContextMenuEvent* pEvent);

private:
    void showTabBar();
    void updateMainWindowTitle();

};

#endif // CONNECTIONSPANE_H

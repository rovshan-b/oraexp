#ifndef CONNECTIONSPANE_H
#define CONNECTIONSPANE_H

#include "widgets/tabwidget.h"
#include <QPointer>
#include "enums.h"

class DbConnection;
class ConnectionPage;
class DbConnectionInfo;
class ReconnectDialog;
class ConnectionPageObject;

class ConnectionsPane : public TabWidget
{
    Q_OBJECT
public:
    explicit ConnectionsPane(QWidget *parent = 0);

    void addConnection(const QString &uuid = QString());
    ConnectionPage *currentConnectionPage() const;

    bool isBusy() const;

    bool activateChildWidget(ConnectionPage *cnPage, ConnectionPageObject *obj);

signals:
    void connectionPaneStateChanged();
    void connectionPageStateChanged();

    void canExit();

public slots:
    bool closeAll(bool exiting = false);
    void closeCurrentTab();
    void closeTab(int index);
    void popOutTab();
    void ctrlTabPressed();
    void setTabTitle(QWidget *tab, DbConnectionInfo *connectionInfo);

    void tabBusyStateChanged(ConnectionPage *cnPage, bool busy);

    void reconnect(OraExp::ReconnectMode reconnectMode);

private slots:
    void checkConnectionCountAndExit();
    void disconnected(DbConnection *db);

protected:
    void contextMenuEvent(QContextMenuEvent* pEvent);

private:
    void showTabBar();
    void updateMainWindowTitle();

    QPointer<ReconnectDialog> reconnectDialog;

};

#endif // CONNECTIONSPANE_H

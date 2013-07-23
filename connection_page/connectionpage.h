#ifndef CONNECTIONPAGE_H
#define CONNECTIONPAGE_H

#include <QWidget>
#include "dbuimanager.h"

class ConnectionPageConnectWidget;
class ConnectionPageConnectedWidget;
class CtrlTabData;

class ConnectionPage : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionPage(QWidget *parent = 0);
    virtual ~ConnectionPage();

    bool isConnected() const;

    DbUiManager *getUiManager();

    void addTab(ConnectionPageTab *tab, const QPixmap &icon, const QString &title);
    void addWindow(ConnectionPageObject *window, const QPixmap &icon, const QString &title);

    void closeTab(QWidget *widget);

    ConnectionPageTab *currentConnectionPageTab() const;
    int tabCount() const;
    ConnectionPageTab *tabAt(int index) const;
    QIcon tabIcon(int index) const;
    QString tabText(int index) const;

    bool isTreePaneVisible() const;

    QList<CtrlTabData*> getCtrlTabData() const;
    void setCurrentTab(QWidget *widget);

    ConnectionPageTab *findTabById(const QString &tabId) const;

    QList<ConnectionPageTab*> getTabsByType(const QString &className) const;
    QList<ConnectionPageTab*> getTabsByConnection(DbConnection *db, const QString &className=QString(), int limit = -1);

public slots:
    void closeTab(int index);
    void toggleTreePane();
    void changeTabCaption(ConnectionPageTab *tab, const QString &caption);

signals:
    void connectionPageStateChanged();

private:
    ConnectionPageConnectWidget *connectWidget;
    ConnectionPageConnectedWidget *mainWidget;

    DbUiManager uiManager;
};

#endif // CONNECTIONPAGE_H

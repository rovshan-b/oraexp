#ifndef CONNECTIONPAGE_H
#define CONNECTIONPAGE_H

#include <QMainWindow>
#include "dbuimanager.h"
#include "connectivity/ociexception.h"
#include "connectivity/connectionpool.h"
#include "widgets/tabwidget.h"

class DbConnection;
class QDockWidget;
class TreePane;
class ConnectionPageTab;
class InfoPanel;

class ConnectionPage : public QMainWindow
{
    Q_OBJECT
public:
    explicit ConnectionPage(DbConnection *db, QWidget *parent = 0);
    virtual ~ConnectionPage();

    void prepareTab(ConnectionPageTab *tab);
    void addTab(ConnectionPageTab *tab, const QPixmap &icon, const QString &title);

    DbUiManager *getUiManager();

    void closeTab(QWidget *widget);

    ConnectionPageTab *currentConnectionPage() const;
    int tabCount() const{return this->centralTab->count();}

    bool isTreePaneVisible() const;

    QList<ConnectionPageTab*> getTabsByConnection(DbConnection *db, const QString &className=QString(), int limit = -1);

    static QByteArray currentState;
signals:
    void connectionPageStateChanged();

public slots:
    void closeTab(int index);
    void asyncConnectionReady(DbConnection *db, void *data, bool error, const OciException &ex);
    void tabBusyStateChanged(ConnectionPageTab *tab, bool busy);
    void tabInitializationCompleted(ConnectionPageTab *tab);
    void toggleTreePane();
    void windowStateChanged();
    void restoreWindowState();

private:
    DbConnection *db;
    QDockWidget *treeDock;
    TabWidget *centralTab;
    TreePane *treePane;
    DbUiManager uiManager;

    ConnectionPool connectionPool;

    void connectDockSignals(QDockWidget *dockWidget);
};

#endif // CONNECTIONPAGE_H

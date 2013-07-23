#ifndef CONNECTIONPAGECONNECTEDWIDGET_H
#define CONNECTIONPAGECONNECTEDWIDGET_H

#include <QMainWindow>
#include "dbuimanager.h"
#include "connectivity/ociexception.h"
#include "connectivity/connectionpool.h"
#include "widgets/connectionpagetabwidget.h"

class DbConnection;
class QDockWidget;
class TreePane;
class ConnectionPageObject;
class ConnectionPageWindowObject;
class InfoPanel;
class CtrlTabData;

class ConnectionPageConnectedWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit ConnectionPageConnectedWidget(QWidget *parent = 0);
    virtual ~ConnectionPageConnectedWidget();

    void prepareObject(ConnectionPageObject *obj);
    void addTab(ConnectionPageTab *tab, const QPixmap &icon, const QString &title);
    void addWindow(ConnectionPageObject *window, const QPixmap &icon, const QString &title);

    //DbUiManager *getUiManager();

    void closeTab(QWidget *widget);

    ConnectionPageTab *currentConnectionPageTab() const;
    int tabCount() const{return this->centralTab->count();}
    ConnectionPageTab *tabAt(int index) const;
    QIcon tabIcon(int index) const;
    QString tabText(int index) const;

    bool isTreePaneVisible() const;

    QList<ConnectionPageTab*> getTabsByType(const QString &className) const;
    QList<ConnectionPageTab*> getTabsByConnection(DbConnection *db, const QString &className=QString(), int limit = -1);

    QList<CtrlTabData*> getCtrlTabData() const;
    void setCurrentTab(QWidget *widget);

    ConnectionPageTab *findTabById(const QString &tabId) const;

    static QByteArray currentState;

signals:
    void connectionPageStateChanged();

public slots:
    void closeTab(int index);
    void asyncConnectionReady(DbConnection *db, void *data, bool error, const OciException &ex);
    void tabBusyStateChanged(ConnectionPageObject *obj, bool busy);
    void tabInitializationCompleted(ConnectionPageObject *obj);
    void toggleTreePane();
    void windowStateChanged();
    void restoreWindowState();

    void changeTabCaption(ConnectionPageTab *tab, const QString &caption);

private:
    DbConnection *db;
    QDockWidget *treeDock;
    ConnectionPageTabWidget *centralTab;
    TreePane *treePane;
    DbUiManager uiManager;

    ConnectionPool connectionPool;

    void connectDockSignals(QDockWidget *dockWidget);
    
};

#endif // CONNECTIONPAGECONNECTEDWIDGET_H

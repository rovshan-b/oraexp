#ifndef CONNECTIONPAGECONNECTEDWIDGET_H
#define CONNECTIONPAGECONNECTEDWIDGET_H

#include <QMainWindow>
#include "dbuimanager.h"
#include "connectivity/ociexception.h"
#include "connectivity/connectionpool.h"
#include "widgets/connectionpagetabwidget.h"
#include "interfaces/ideletelistener.h"

class DbConnection;
class QDockWidget;
class TreePane;
class ConnectionPageObject;
class ConnectionPageWindowObject;
class InfoPanel;
class CtrlTabData;
class CodeStructurePane;

class ConnectionPageConnectedWidget : public QMainWindow, public IDeleteListener
{
    Q_OBJECT
public:
    explicit ConnectionPageConnectedWidget(DbConnection *db, DbUiManager *uiManager, QWidget *parent = 0);
    virtual ~ConnectionPageConnectedWidget();

    void prepareObject(ConnectionPageObject *obj);
    void addTab(ConnectionPageTab *tab, const QPixmap &icon, const QString &title);
    void addWindow(ConnectionPageObject *window, const QPixmap &icon, const QString &title);

    void beforeDelete(ConnectionPageObject *obj);

    //DbUiManager *getUiManager();

    void beforeClose();
    void closeTab(QWidget *widget);

    ConnectionPageTab *currentConnectionPageTab() const;
    int tabCount() const{return this->centralTab->count();}
    int indexOf(const ConnectionPageTab *tab) const;
    ConnectionPageTab *tabAt(int index) const;
    QIcon tabIcon(int index) const;
    QString tabText(int index) const;

    bool activateChildWidget(ConnectionPageObject *obj);

    bool isDbTreePaneVisible() const;
    bool isCodeStructurePaneVisible() const;

    QList<ConnectionPageTab*> getTabsByType(const QString &className) const;
    QList<ConnectionPageTab*> getTabsByConnection(DbConnection *db, const QString &className=QString(), int limit = -1);

    QList<CtrlTabData*> getCtrlTabData() const;
    void setCurrentTab(QWidget *widget);
    void setCurrentIndex(int ix);

    ConnectionPageTab *findTabById(const QString &tabId) const;

    const ConnectionPool *getConnectionPool() const;

    static QByteArray currentState;

signals:
    void connectionPageStateChanged();

    void busyStateChanged(bool busy);

public slots:
    void closeCurrentTab();
    void closeTab(int index);
    void asyncConnectionReady(DbConnection *db, void *data, bool error, const OciException &ex);
    void tabBusyStateChanged(ConnectionPageObject *obj, bool busy);
    void tabInitializationCompleted(ConnectionPageObject *obj);

    void toggleDbTreePane();
    void toggleCodeStructurePane();
    void showCodeStructurePane();
    void hideCodeStructurePane();

    void windowStateChanged();
    void restoreWindowState();

    void changeTabCaption(ConnectionPageTab *tab, const QString &caption);

private slots:
    void connectionPoolWorking(bool isWorking);

private:
    DbConnection *db;
    QDockWidget *dbTreeDock;
    QDockWidget *codeTreeDock;
    ConnectionPageTabWidget *centralTab;
    TreePane *dbTreePane;
    CodeStructurePane *codeTreePane;
    DbUiManager *uiManager;

    ConnectionPool connectionPool;

    int busyCounter;

    QList<ConnectionPageObject*> childObjects;

    void createDbTreeDock();
    void createCodeTreeDock();
    void connectDockSignals(QDockWidget *dockWidget);
    
};

#endif // CONNECTIONPAGECONNECTEDWIDGET_H

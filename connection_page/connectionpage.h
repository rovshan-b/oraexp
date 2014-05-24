#ifndef CONNECTIONPAGE_H
#define CONNECTIONPAGE_H

#include <QWidget>
#include "dbuimanager.h"
#include "models/connectionlistmodel.h"

class ConnectionPageConnectWidget;
class ConnectionPageConnectedWidget;
class CtrlTabData;
class ConnectionPool;

class ConnectionPage : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionPage(const QString &connectionUuid,
                            QWidget *parent = 0);
    virtual ~ConnectionPage();

    void focusReady();

    bool isConnected() const;

    DbUiManager *getUiManager();

    void addTab(ConnectionPageTab *tab, const QPixmap &icon, const QString &title);
    void addWindow(ConnectionPageObject *window, const QPixmap &icon, const QString &title);

    void beforeClose();
    void closeTab(QWidget *widget);

    ConnectionPageTab *currentConnectionPageTab() const;
    int tabCount() const;
    int indexOf(const ConnectionPageTab *tab) const;
    ConnectionPageTab *tabAt(int index) const;
    QIcon tabIcon(int index) const;
    QString tabText(int index) const;

    bool activateChildWidget(ConnectionPageObject *obj);

    bool isDbTreePaneVisible() const;
    bool isCodeStructurePaneVisible() const;

    QList<CtrlTabData*> getCtrlTabData() const;
    void setCurrentTab(QWidget *widget);
    void setCurrentIndex(int ix);

    ConnectionPageTab *findTabById(const QString &tabId) const;

    QList<ConnectionPageTab*> getTabsByType(const QString &className) const;
    QList<ConnectionPageTab*> getTabsByConnection(DbConnection *db, const QString &className=QString(), int limit = -1);

    CodeStructurePane *getCodeStructurePane() const;

    bool isBusy() const {return this->busy;}

    const ConnectionPool *getConnectionPool() const;

    OraExp::ConnectionEnvironment getConnectionEnvironment() const;

signals:
    void busyStateChanged(ConnectionPage *cnPage, bool busy);

public slots:
    void closeTab(int index);

    void toggleDbTreePane();
    void toggleCodeStructurePane();

    void changeTabCaption(ConnectionPageTab *tab, const QString &caption);

private slots:
    void connected(DbConnection *db, DbConnectionInfo *connectionInfo);
    void childBusyStateChanged(bool busy);

signals:
    void connectionPageStateChanged();
    void setTitle(QWidget *tab, DbConnectionInfo *connectionInfo);

private:
    ConnectionPageConnectWidget *connectWidget;
    ConnectionPageConnectedWidget *mainWidget;

    DbUiManager uiManager;

    OraExp::ConnectionEnvironment currentEnvironment;

    bool busy;
};

#endif // CONNECTIONPAGE_H

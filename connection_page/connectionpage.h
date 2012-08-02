#ifndef CONNECTIONPAGE_H
#define CONNECTIONPAGE_H

#include <QWidget>
#include "dbuimanager.h"
#include "connectivity/ociexception.h"
#include "connectivity/connectionpool.h"
#include "widgets/tabwidget.h"

class DbConnection;
class QDockWidget;
class TreePane;
class ConnectionPageTab;

class ConnectionPage : public QWidget
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

signals:
    void connectionPageStateChanged();

public slots:
    void closeTab(int index);
    void asyncConnectionReady(DbConnection *db, void *data, bool error, const OciException &ex);
    void tabBusyStateChanged(ConnectionPageTab *tab, bool busy);

private:
    DbConnection *db;
    QDockWidget *treeDock;
    TabWidget *centralTab;
    TreePane *treePane;
    DbUiManager uiManager;

    ConnectionPool connectionPool;
};

#endif // CONNECTIONPAGE_H

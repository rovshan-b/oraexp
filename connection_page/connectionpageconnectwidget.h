#ifndef CONNECTIONPAGECONNECTWIDGET_H
#define CONNECTIONPAGECONNECTWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QSharedPointer>
#include "connectivity/ociexception.h"
#include "models/connectionlistmodel.h"

class QSplitter;
class LineEditWithClearButton;
class DataTable;
class ConnectionEditor;
class AsyncConnect;
class DbConnection;
class DbConnectionInfo;
class QSortFilterProxyModel;

class ConnectionPageConnectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectionPageConnectWidget(const QSharedPointer<ConnectionListModel> &model,
                                         const QString &connectionUuid,
                                         QWidget *parent = 0);

    QSharedPointer<ConnectionListModel> getConnectionListModel() const;

    void focusReady();

signals:
    void connected(DbConnection *db, DbConnectionInfo *connectionInfo);

    void busyStateChanged(bool busy);

private slots:
    void addConnection();
    void saveConnection();
    void deleteConnection();
    void connectToDb(bool test = false);
    void testConnection();

    void connectToInitialUuid();

    void connectionActivated ( const QModelIndex & index );

    void connectionEstablished(AsyncConnect *thread, DbConnection *db, void *data, bool error=false, const OciException &ex=OciException());

    void filterList(const QString &pattern);

protected:
    void resizeEvent ( QResizeEvent * event );
    bool eventFilter(QObject *obj, QEvent *event);
    void keyPressEvent ( QKeyEvent * event );

private slots:
    void currentConnectionChanged ( const QModelIndex & current, const QModelIndex & previous );
    void connectionModified(DbConnectionInfo *connection);
    
private:
    void createUi();

    void createConnectionList(QSplitter *splitter);
    void createConnectionEditor(QSplitter *splitter);

    void updateCurrentConnection();

    void loadConnectionList();
    void saveConnectionList();

    void selectConnection(bool first);
    void selectInTable(DbConnectionInfo *connection);

    LineEditWithClearButton *filterEditor;
    DataTable *table;
    QSharedPointer<ConnectionListModel> model;
    QSortFilterProxyModel *proxyModel;
    ConnectionEditor *connectionEditor;

    bool testMode;
    bool deleteOnFail;

    bool modifyingConnection;

    QString initialConnectionUuid;
    
};

#endif // CONNECTIONPAGECONNECTWIDGET_H

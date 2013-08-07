#ifndef CONNECTIONSELECTORWIDGET_H
#define CONNECTIONSELECTORWIDGET_H

#include "lineeditwithbutton.h"

class DbConnection;
class DbConnectionInfo;
class IQueryScheduler;

class ConnectionSelectorWidget : public LineEditWithButton
{
    Q_OBJECT
public:
    explicit ConnectionSelectorWidget(QWidget *parent = 0);
    ~ConnectionSelectorWidget();

    DbConnection *getDb() const {return this->db;}

    void setQueryScheduler(IQueryScheduler *queryScheduler);

signals:
    void connectionEstablished(DbConnection *db);

private slots:
    void connectToServer();

    void connected(DbConnection *db, DbConnectionInfo *connectionInfo);

    void connectDialogClosed();

private:
    DbConnection *db;

    //used only for increasing and decreasing reference count in order to keep parent widget's busy status while connecting
    IQueryScheduler *queryScheduler;

    void cleanup();

};

#endif // CONNECTIONSELECTORWIDGET_H

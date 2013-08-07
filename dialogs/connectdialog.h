#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

class ConnectionPageConnectWidget;
class DbConnection;
class DbConnectionInfo;

class ConnectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectDialog(QWidget *parent = 0);

signals:
    void connected(DbConnection *db, DbConnectionInfo *connectionInfo);

    void closed();

protected:
    virtual void closeEvent(QCloseEvent *e);

private slots:
    virtual void reject ();

    void connectionEstablished(DbConnection *db, DbConnectionInfo *connectionInfo);

private:
    ConnectionPageConnectWidget *connectWidget;

    bool canClose() const;
    
};

#endif // CONNECTDIALOG_H

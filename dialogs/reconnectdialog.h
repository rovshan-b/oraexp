#ifndef RECONNECTDIALOG_H
#define RECONNECTDIALOG_H

#include <QDialog>
#include "util/triple.h"
#include "connectivity/ociexception.h"
#include <QModelIndex>

class DataTable;
class QStandardItemModel;
class ConnectionPage;
class ConnectionPageObject;
class DbConnection;

class ReconnectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ReconnectDialog(QWidget *parent = 0);
    ~ReconnectDialog();

    void setConnections(const QList<Triple<ConnectionPage *, ConnectionPageObject *, DbConnection *> *> &connections);

    virtual void reject ();

protected:
    virtual void closeEvent(QCloseEvent *e);
    virtual bool canClose();

    void resizeEvent ( QResizeEvent * event );

private slots:
    void reconnected(DbConnection *db, bool error = false, const OciException &ex = OciException());

    void itemActivated(const QModelIndex &index);

private:
    DataTable *table;
    QStandardItemModel *model;

    QList<Triple<ConnectionPage *, ConnectionPageObject *, DbConnection *> *> connections;

    int activeThreadCount;

    int indexOf(DbConnection *db);
    
};

#endif // RECONNECTDIALOG_H

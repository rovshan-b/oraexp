#ifndef RECONNECTDIALOG_H
#define RECONNECTDIALOG_H

#include <QDialog>
#include "util/triple.h"

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
    void startChecking();

private:
    DataTable *table;
    QStandardItemModel *model;

    QList<Triple<ConnectionPage *, ConnectionPageObject *, DbConnection *> *> connections;
    
};

#endif // RECONNECTDIALOG_H

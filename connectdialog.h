#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

class DbConnection;

namespace Ui {
    class ConnectDialog;
}

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

    DbConnection *getConnection() const;

    void saveConnectionsToConfig();
    void loadConnectionsFromConfig();

private:
    Ui::ConnectDialog *ui;

    DbConnection *currentDb;

    void showConnectionEditorDialog(DbConnection *db, int currentRow);
    void disableSortingAndSaveConnectionInfo(DbConnection *db, int currentRow);
    void saveConnectionInfo(DbConnection *db, int currentRow);
    bool promptForPassword(DbConnection *db);
    void tryToEstablishConnection(DbConnection *db);

    //void centerWindow();

private slots:
    void on_connectButton_clicked();
    void on_addConnectionButton_clicked();
    void on_editConnectionButton_clicked();
    void on_deleteConnectionButton_clicked();
    void tableActivated();

    void addConnection();
    void editConnection();
    void deleteConnection();
    void connectToServer();

};

#endif // CONNECTDIALOG_H

#ifndef CONNECTIONEDITORDIALOG_H
#define CONNECTIONEDITORDIALOG_H

#include <QDialog>
#include <QString>

class QPushButton;
class DbConnection;

namespace Ui {
    class ConnectionEditorDialog;
}

class ConnectionEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionEditorDialog(QWidget *parent = 0);
    ~ConnectionEditorDialog();

    DbConnection *getDbConnection();
    void setDbConnection(DbConnection *db);

private:
    Ui::ConnectionEditorDialog *ui;
    QPushButton *testConnectionButton;

    void populateEnvironmentNames();
    void populateTnsNames();
    bool isFormValid();

public slots:
    void testConnection();
    void validateForm();

};

#endif // CONNECTIONEDITORDIALOG_H

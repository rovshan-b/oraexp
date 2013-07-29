#ifndef PASSWORDENTRYDIALOG_H
#define PASSWORDENTRYDIALOG_H

#include <QDialog>
class QPushButton;

namespace Ui {
    class PasswordEntryDialog;
}

class PasswordEntryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PasswordEntryDialog(QWidget *parent = 0);
    ~PasswordEntryDialog();

    Ui::PasswordEntryDialog *ui;

    QString password() const;
    bool savePassword() const;

public slots:
    void validateForm();

private:
    bool isFormValid();

    QPushButton *okButton;
};

#endif // PASSWORDENTRYDIALOG_H

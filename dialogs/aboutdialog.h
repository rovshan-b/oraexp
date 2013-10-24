#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();
    
private slots:
    void on_infoLabel_linkActivated(const QString &link);

    void on_closeButton_clicked();

    void on_creditsButton_clicked();

    void on_websiteLabel_linkActivated(const QString &link);

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H

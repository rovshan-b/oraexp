#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class QBoxLayout;

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = 0);
    
private slots:
    void linkClicked(const QString &url);

    void showCreditsDialog();

private:
    void createTopPane(QBoxLayout *layout);
    void createMiddlePane(QBoxLayout *layout);
    void createBottomPane(QBoxLayout *layout);
    
};

#endif // ABOUTDIALOG_H

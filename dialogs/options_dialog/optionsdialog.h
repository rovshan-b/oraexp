#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

class QListWidget;
class QStackedWidget;
class QLabel;
class QBoxLayout;

class OptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit OptionsDialog(QWidget *parent = 0);

private slots:
    virtual void accept ();
    void apply();

    void changeTab(int tabIx);
    
private:
    QListWidget *sidePane;
    QStackedWidget *tab;

    QLabel *titleLabel;

    void createLeftPane(QBoxLayout *layout);
    void createRightPane(QBoxLayout *layout);
    void addTabs();
};

#endif // OPTIONSDIALOG_H

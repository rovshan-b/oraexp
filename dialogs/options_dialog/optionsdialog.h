#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

class QListWidget;
class QCheckBox;
class QBoxLayout;

class OptionsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit OptionsDialog(QWidget *parent = 0);

private slots:
    virtual void accept ();
    void apply();
    
private:
    QListWidget *sidePane;
    QCheckBox *useSeparateSessionsCheckBox;

    void createLeftPane(QBoxLayout *layout);
    void createRightPane(QBoxLayout *layout);

    void initializeValues();
};

#endif // OPTIONSDIALOG_H

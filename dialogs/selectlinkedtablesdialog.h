#ifndef SELECTLINKEDTABLESDIALOG_H
#define SELECTLINKEDTABLESDIALOG_H

#include <QDialog>

class QRadioButton;
class QLineEdit;
class QSpinBox;

class SelectLinkedTablesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectLinkedTablesDialog(QWidget *parent = 0);
    
private:
    QRadioButton *parentTablesRadio;
    QRadioButton *childTablesRadio;
    QRadioButton *bothRadio;

    QLineEdit *patternEditor;
    QSpinBox *maxLevelEditor;
    
};

#endif // SELECTLINKEDTABLESDIALOG_H

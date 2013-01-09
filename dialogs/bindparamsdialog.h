#ifndef BINDPARAMSDIALOG_H
#define BINDPARAMSDIALOG_H

#include <QDialog>

class BindParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BindParamsDialog(const QStringList &bindParams, QWidget *parent);
    
private:
    QStringList bindParams;

    void createUi();
    QWidget *createForm();
    
};

#endif // BINDPARAMSDIALOG_H

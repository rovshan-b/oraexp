#ifndef BINDPARAMSDIALOG_H
#define BINDPARAMSDIALOG_H

#include <QDialog>

class Param;
class BindParamEditorWidget;

class BindParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BindParamsDialog(const QStringList &bindParams, QWidget *parent);

    QList<Param*> getParams() const;
    
private:
    QStringList bindParams;
    QList<BindParamEditorWidget*> editors;

    void createUi();
    QWidget *createForm();
    
};

#endif // BINDPARAMSDIALOG_H

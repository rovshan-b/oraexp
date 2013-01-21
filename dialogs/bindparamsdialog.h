#ifndef BINDPARAMSDIALOG_H
#define BINDPARAMSDIALOG_H

#include <QDialog>
#include <QHash>
#include "beans/bindparaminfo.h"

class Param;
class BindParamEditorWidget;
class BindParamInfo;

class BindParamsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BindParamsDialog(const QStringList &bindParams, const QList<BindParamInfo::BindParamType> &suggestedParamTypes,
                              const QHash<QString, BindParamInfo *> &paramHistory, QWidget *parent);

    QList<Param*> getParams() const;

public slots:
    virtual void accept();
    
private:
    QStringList bindParams;
    QList<BindParamInfo::BindParamType> suggestedParamTypes;
    QList<BindParamEditorWidget*> editors;
    QHash<QString, BindParamInfo *> paramHistory;

    void createUi();
    QWidget *createForm();

    BindParamEditorWidget *controlToSetFocus;
    
};

#endif // BINDPARAMSDIALOG_H

#ifndef BINDPARAMEDITORWIDGET_H
#define BINDPARAMEDITORWIDGET_H

#include <QWidget>
#include "beans/bindparaminfo.h"

class QComboBox;
class Param;
class QRegExpValidator;

class BindParamEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BindParamEditorWidget(QWidget *parent = 0);

    bool isEditorEnabled() const;
    void setFocusToEditor();
    void setBindParamType(BindParamInfo::BindParamType type);
    BindParamInfo::BindParamType getBindParamType() const;
    QString getBindParamValue() const;

    void setBindParamInfo(BindParamInfo *paramInfo);

    Param *createParam(const QString &paramName);
    
private slots:
    void paramTypeChanged(int newType);

private:
    QComboBox *paramTypeCombo;
    QComboBox *valueEditor;
    QComboBox *paramDirectionCombo;

    //QRegExpValidator *dateValidator;
    
};

#endif // BINDPARAMEDITORWIDGET_H

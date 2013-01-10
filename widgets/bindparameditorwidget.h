#ifndef BINDPARAMEDITORWIDGET_H
#define BINDPARAMEDITORWIDGET_H

#include <QWidget>

class QComboBox;
class Param;
class QRegExpValidator;

class BindParamEditorWidget : public QWidget
{
    Q_OBJECT
public:
    enum BindParamType
    {
        StringOrNumber,
        Date,
        Cursor
    };

    explicit BindParamEditorWidget(QWidget *parent = 0);

    void setFocusToEditor();
    void setBindParamType(BindParamType type);

    Param *createParam(const QString &paramName);
    
private slots:
    void paramTypeChanged(int newType);

private:
    QComboBox *paramTypeCombo;
    QComboBox *valueEditor;
    QComboBox *paramDirectionCombo;

    QRegExpValidator *dateValidator;
    
};

#endif // BINDPARAMEDITORWIDGET_H

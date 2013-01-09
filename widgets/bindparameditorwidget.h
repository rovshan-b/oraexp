#ifndef BINDPARAMEDITORWIDGET_H
#define BINDPARAMEDITORWIDGET_H

#include <QWidget>

class QComboBox;

class BindParamEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BindParamEditorWidget(QWidget *parent = 0);

    void setFocusToEditor();
    
private:
    QComboBox *paramTypeCombo;
    QComboBox *valueEditor;
    QComboBox *paramDirectionCombo;
    
};

#endif // BINDPARAMEDITORWIDGET_H

#ifndef EXTENTSIZEEDITORWIDGET_H
#define EXTENTSIZEEDITORWIDGET_H

#include <QWidget>
#include "enums.h"

class QLineEdit;
class QComboBox;
class QCheckBox;

class ExtentSizeEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExtentSizeEditorWidget(bool displayUnitCombo, bool addUnlimitedOption, QWidget *parent = 0);

    QString text() const;
    void setText(const QString &text);

    OraExp::ExtentUnit unit() const;
    void setUnit(OraExp::ExtentUnit unit);

    bool isUnlimited() const;
    void setUnlimited(bool unlimited);

    QLineEdit *lineEdit() const;
    QComboBox *editorComboBox() const;
    //QCheckBox *checkBox() const;

signals:
    void changed();

//private slots:
//    void unlimitedCheckBoxClicked(bool selected);

private:
    QLineEdit *extentEditor;
    QComboBox *extentEditorCombo;
    QComboBox *extentUnitCombo;
    //QCheckBox *unlimitedCheckBox;

};

#endif // EXTENTSIZEEDITORWIDGET_H

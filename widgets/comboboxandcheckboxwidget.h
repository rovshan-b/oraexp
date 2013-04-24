#ifndef COMBOBOXANDCHECKBOXWIDGET_H
#define COMBOBOXANDCHECKBOXWIDGET_H

#include <QWidget>

class QComboBox;
class QCheckBox;

class ComboBoxAndCheckBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ComboBoxAndCheckBoxWidget(const QString &checkBoxTitle,
                                       QWidget *parent = 0);
    
    QComboBox *comboBox() const;
    QCheckBox *checkBox() const;

signals:
    void changed();

private:
    QComboBox *combo;
    QCheckBox *check;
    
};

#endif // COMBOBOXANDCHECKBOXWIDGET_H

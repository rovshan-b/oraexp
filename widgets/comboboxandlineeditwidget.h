#ifndef COMBOBOXANDLINEEDITWIDGET_H
#define COMBOBOXANDLINEEDITWIDGET_H

#include <QWidget>
class QComboBox;
class QLineEdit;

class ComboBoxAndLineEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ComboBoxAndLineEditWidget(bool onlyNumericInput, const QString &placeholderText="", const QStringList &stringsToDisable=QStringList(), QWidget *parent = 0);

    QComboBox *comboBox() const;
    QLineEdit *lineEdit() const;

signals:
    void changed();

protected:
    virtual void changeEvent(QEvent *event);

private:
    QComboBox *dropdown;
    QLineEdit *editor;

    QString placeholderText;
    QStringList stringsToDisable;

private slots:
    void comboBoxIndexChanged();

};

#endif // COMBOBOXANDLINEEDITWIDGET_H

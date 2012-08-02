#ifndef LINEEDITANDCHECKBOXWIDGET_H
#define LINEEDITANDCHECKBOXWIDGET_H

#include <QWidget>

class QLineEdit;
class QCheckBox;

class LineEditAndCheckBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LineEditAndCheckBoxWidget(const QString &checkBoxTitle, QWidget *parent = 0);

    QLineEdit *lineEdit() const;
    QCheckBox *checkBox() const;

signals:
    void changed();

private:
    QLineEdit *editor;
    QCheckBox *check;
};

#endif // LINEEDITANDCHECKBOXWIDGET_H

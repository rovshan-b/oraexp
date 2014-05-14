#ifndef COMBOBOXWITHBUTTON_H
#define COMBOBOXWITHBUTTON_H

#include <QWidget>
#include "dbitemlistcombobox.h"

class QPushButton;
class QLineEdit;

class ComboBoxWithButton : public QWidget
{
    Q_OBJECT
public:
    explicit ComboBoxWithButton(const QString &initialValue = QString(), const QString &iconName = QString(), QWidget *parent = 0);

    DbItemListComboBox *comboBox() const;
    QPushButton *browseButton() const;

    QLineEdit *lineEdit() const;

signals:
    void buttonClicked(ComboBoxWithButton *senderWidget);

private slots:
    void browseButtonClicked();

private:
    DbItemListComboBox *combo;
    QPushButton *button;
};

#endif // COMBOBOXWITHBUTTON_H

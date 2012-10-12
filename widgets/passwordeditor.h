#ifndef PASSWORDEDITOR_H
#define PASSWORDEDITOR_H

#include "lineeditandcheckboxwidget.h"

class PasswordEditor : public LineEditAndCheckBoxWidget
{
    Q_OBJECT
public:
    PasswordEditor(QWidget *parent = 0);

private slots:
    void showPassword();
};

#endif // PASSWORDEDITOR_H

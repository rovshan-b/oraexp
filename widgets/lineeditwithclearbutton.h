#ifndef LINEEDITWITHCLEARBUTTON_H
#define LINEEDITWITHCLEARBUTTON_H

#include <QWidget>

class QLineEdit;

class LineEditWithClearButton : public QWidget
{
    Q_OBJECT
public:
    explicit LineEditWithClearButton(QWidget *parent = 0);
    
    QLineEdit *lineEdit() const {return this->editor;}

private:
    QLineEdit *editor;
    
};

#endif // LINEEDITWITHCLEARBUTTON_H

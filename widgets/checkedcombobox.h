#ifndef CHECKEDCOMBOBOX_H
#define CHECKEDCOMBOBOX_H

#include <QComboBox>

class CheckedComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CheckedComboBox(QWidget *parent = 0);

    bool eventFilter(QObject *, QEvent *);

    virtual void showPopup ();
    virtual void hidePopup ();

};

#endif // CHECKEDCOMBOBOX_H

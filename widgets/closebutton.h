#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <QAbstractButton>

class CloseButton : public QAbstractButton
{
    Q_OBJECT

public:
    CloseButton(QWidget *parent = 0);

    QSize sizeHint() const;
    inline QSize minimumSizeHint() const
    { return sizeHint(); }
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void paintEvent(QPaintEvent *event);
};

#endif // CLOSEBUTTON_H

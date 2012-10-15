#include "centeredcheckboxwidget.h"
#include <QtGui>
#include <QDebug>

CenteredCheckBoxWidget::CenteredCheckBoxWidget(QWidget *parent) :
    QWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    setAutoFillBackground(true);

    QHBoxLayout *layout=new QHBoxLayout();
    checkbox=new QCheckBox();
    layout->addWidget(checkbox);
    layout->setAlignment(checkbox, Qt::AlignHCenter | Qt::AlignVCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    setLayout(layout);

    setFocusProxy(checkbox);
    //checkbox->installEventFilter(this);
}

QCheckBox *CenteredCheckBoxWidget::checkBox() const
{
    return checkbox;
}
/*
bool CenteredCheckBoxWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::FocusIn && watched==checkbox){
        QFocusEvent *focusEvent = static_cast<QFocusEvent*>(event);
        qDebug() << focusEvent->reason();
        if(focusEvent->reason()==Qt::MouseFocusReason){
            checkbox->setChecked(!checkbox->isChecked());
        }
    }

    return QWidget::eventFilter(watched, event);
}
*/

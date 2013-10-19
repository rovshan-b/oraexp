#include "messagewidget.h"
#include <QtGui>

MessageWidget::MessageWidget(const QString &message, QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout();

    label = new QLabel(message);
    layout->addWidget(label);
    layout->setAlignment(label, Qt::AlignCenter);

    setLayout(layout);
}

void MessageWidget::setMessage(const QString &message)
{
    label->setText(message);
}

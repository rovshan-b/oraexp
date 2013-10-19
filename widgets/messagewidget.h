#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>

class QLabel;

class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessageWidget(const QString &message, QWidget *parent = 0);

    void setMessage(const QString &message);
    
private:
    QLabel *label;
    
};

#endif // MESSAGEWIDGET_H

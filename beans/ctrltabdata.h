#ifndef CTRLTABDATA_H
#define CTRLTABDATA_H

#include <QIcon>
#include <QString>

class QWidget;

class CtrlTabData
{
public:
    CtrlTabData(const QIcon &icon, const QString &title, QWidget *widget);

    QIcon icon;
    QString title;
    QWidget *widget;
};

#endif // CTRLTABDATA_H

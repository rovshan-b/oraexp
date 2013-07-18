#ifndef DYNAMICWINDOWINFO_H
#define DYNAMICWINDOWINFO_H

#include <QString>
#include <QList>
#include "dynamicwidgetinfo.h"

class DynamicWindowInfo
{
public:
    DynamicWindowInfo();
    virtual ~DynamicWindowInfo();

    QString caption;
    QString icon;
    QString type;
    QString scriptFileName;

    QList< DynamicWidgetInfo* > widgetInfos;
};

#endif // DYNAMICWINDOWINFO_H

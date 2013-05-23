#ifndef DYNAMICWINDOWINFO_H
#define DYNAMICWINDOWINFO_H

#include <QString>
#include <QList>
#include <QHash>

class DynamicWindowInfo
{
public:
    DynamicWindowInfo();

    QString caption;

    QList< QHash<QString,QString> > widgetInfos;
};

#endif // DYNAMICWINDOWINFO_H

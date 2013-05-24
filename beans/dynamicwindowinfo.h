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
    QString type;
    QString scriptFileName;

    QList< QHash<QString,QString> > widgetInfos;
};

#endif // DYNAMICWINDOWINFO_H

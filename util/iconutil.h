#ifndef ICONUTIL_H
#define ICONUTIL_H

#include <QIcon>

class IconUtil
{
public:
    IconUtil();

    static QIcon getEnvironmentIcon(const QString &environmentName);
    static QPixmap getIcon(const QString &iconName);
};

#endif // ICONUTIL_H

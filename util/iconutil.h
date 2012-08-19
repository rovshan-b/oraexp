#ifndef ICONUTIL_H
#define ICONUTIL_H

#include <QIcon>

class IconUtil
{
public:
    IconUtil();

    static QIcon getEnvironmentIcon(const QString &environmentName);
    static QPixmap getIcon(const QString &iconName);
    static QPixmap loadIconFromFile(const QString &iconName);
    static QPixmap overlayIcons(const QPixmap &first, const QPixmap &second);
};

#endif // ICONUTIL_H

#ifndef ICONUTIL_H
#define ICONUTIL_H

#include <QIcon>
#include "enums.h"

class IconUtil
{
public:
    IconUtil();

    static QIcon getEnvironmentIcon(OraExp::ConnectionEnvironment environment, bool big = false);
    static QPixmap getIcon(const QString &iconName);
    static QPixmap loadIconFromFile(const QString &iconName);
    static QPixmap overlayIcons(const QPixmap &first, const QPixmap &second);
};

#endif // ICONUTIL_H

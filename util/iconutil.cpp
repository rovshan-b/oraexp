#include "iconutil.h"
#include <QObject>
#include <QPixmapCache>
#include <stdexcept>
#include <QtDebug>

IconUtil::IconUtil()
{
}

QIcon IconUtil::getEnvironmentIcon(const QString &environmentName)
{
    if(environmentName==QObject::tr("Test")){
        return QIcon(":/images/green.png");
    }else if(environmentName==QObject::tr("Development")){
        return QIcon(":/images/yellow.png");
    }else{
        return QIcon(":/images/red.png");
    }
}

QPixmap IconUtil::getIcon(const QString &iconName)
{
    QString iconPath=QString(":/images/%1.png").arg(iconName);

    QPixmap result;

    if (!QPixmapCache::find(iconPath, &result)) {
        QIcon icon(iconPath);
        if(icon.isNull()){
            qDebug() << "icon not found: " << iconPath;
            throw std::runtime_error("icon file not found");
        }

        result=icon.pixmap(QSize(16,16));
        QPixmapCache::insert(iconPath, result);
    }

    return result;
}

#ifndef METAOBJECTHELPER_H
#define METAOBJECTHELPER_H

#include <QObject>
#include <QList>
#include <QGenericArgument>

class MetaObjectHelper
{
public:
    static bool invokeMethod(QObject *obj, const QString &member,
                             const QList<QGenericArgument> &arguments);

private:
    MetaObjectHelper();
};

#endif // METAOBJECTHELPER_H

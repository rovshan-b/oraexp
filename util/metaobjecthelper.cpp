#include "metaobjecthelper.h"
#include <QDebug>

MetaObjectHelper::MetaObjectHelper()
{
}

bool MetaObjectHelper::invokeMethod(QObject *obj, const QString &member, const QList<QGenericArgument> &arguments)
{
    Q_ASSERT(arguments.size()<=10);

    QGenericArgument args[10];

    for(int i=0; i<arguments.size(); ++i){
        args[i] = arguments.at(i);
    }

    qDebug() << "invoking slot names" << member << "on object" << obj << "with" << arguments.size() << "arguments";
    return QMetaObject::invokeMethod(obj, member.toStdString().c_str(), Qt::DirectConnection,
                                     args[0], args[1], args[2], args[3], args[4],
                                     args[5], args[6], args[7], args[8], args[9]);
}

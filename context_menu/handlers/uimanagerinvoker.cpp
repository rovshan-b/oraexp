#include "uimanagerinvoker.h"
#include "util/metaobjecthelper.h"

UiManagerInvoker::UiManagerInvoker()
{
}

void UiManagerInvoker::handle(QObject *resultListener, const QHash<QString, QString> &properties)
{
    QList<QGenericArgument> arguments;
    QList<QString*> allocatedStrings;

    int i=0;
    while(++i){
        QString propName = QString("attribute.param%1").arg(i);
        if(!properties.contains(propName)){
            break;
        }

        QString propValue = properties.value(propName);
        if(propValue.startsWith("arg:")){
            propValue = properties.value(propValue.mid(4));
        }

        bool isNumeric;
        int numValue = propValue.toInt(&isNumeric);

        if(isNumeric){
            arguments.append(Q_ARG(int, numValue));
        }else{
            QString *strVal = new QString(propValue);
            allocatedStrings.append(strVal);
            arguments.append(Q_ARG(QString, *strVal));
        }
    }

    MetaObjectHelper::invokeMethod(uiManager,
                                   properties["attribute.slotName"],
                                   arguments);
    qDeleteAll(allocatedStrings);
}

#ifndef DYNAMICWIDGETINFO_H
#define DYNAMICWIDGETINFO_H

#include <QHash>
#include <QDomNodeList>

class DynamicWidgetInfo
{
public:
    DynamicWidgetInfo();

    QHash<QString,QString> attributes;

    QDomNodeList childNodes;
};

#endif // DYNAMICWIDGETINFO_H

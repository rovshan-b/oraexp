#ifndef GENERICOBJECTCONTEXTMENUUTIL_H
#define GENERICOBJECTCONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class IActionPropertySetter;

class GenericObjectContextMenuUtil
{
public:
    GenericObjectContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        DbUiManager *uiManager,
                                        IActionPropertySetter *propertySetter);
};

#endif // GENERICOBJECTCONTEXTMENUUTIL_H

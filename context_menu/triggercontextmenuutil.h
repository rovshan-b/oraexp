#ifndef TRIGGERCONTEXTMENUUTIL_H
#define TRIGGERCONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class TriggerContextMenuUtil
{
public:
    TriggerContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // TRIGGERCONTEXTMENUUTIL_H

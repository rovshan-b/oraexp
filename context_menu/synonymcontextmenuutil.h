#ifndef SYNONYMCONTEXTMENUUTIL_H
#define SYNONYMCONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class SynonymContextMenuUtil
{
public:
    SynonymContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // SYNONYMCONTEXTMENUUTIL_H

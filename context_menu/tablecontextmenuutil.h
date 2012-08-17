#ifndef TABLECONTEXTMENUUTIL_H
#define TABLECONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class TableContextMenuUtil
{
public:
    TableContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // TABLECONTEXTMENUUTIL_H

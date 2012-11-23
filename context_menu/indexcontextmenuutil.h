#ifndef INDEXCONTEXTMENUUTIL_H
#define INDEXCONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class IndexContextMenuUtil
{
public:
    IndexContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // INDEXCONTEXTMENUUTIL_H

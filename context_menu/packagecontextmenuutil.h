#ifndef PACKAGECONTEXTMENUUTIL_H
#define PACKAGECONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class PackageContextMenuUtil
{
public:
    PackageContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // PACKAGECONTEXTMENUUTIL_H

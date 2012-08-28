#ifndef TYPECONTEXTMENUUTIL_H
#define TYPECONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class TypeContextMenuUtil
{
public:
    TypeContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // TYPECONTEXTMENUUTIL_H

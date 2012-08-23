#ifndef FUNCTIONCONTEXTMENUUTIL_H
#define FUNCTIONCONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class FunctionContextMenuUtil
{
public:
    FunctionContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // FUNCTIONCONTEXTMENUUTIL_H

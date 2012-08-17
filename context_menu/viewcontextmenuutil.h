#ifndef VIEWCONTEXTMENUUTIL_H
#define VIEWCONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class ViewContextMenuUtil
{
public:
    ViewContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // VIEWCONTEXTMENUUTIL_H

#ifndef USERCONTEXTMENUUTIL_H
#define USERCONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class UserContextMenuUtil
{
public:
    UserContextMenuUtil();

    static QList<QAction *> getActionsForObject(const QString &schemaName,
                                         const QString &objectName,
                                         const DbTreeModel::DbTreeNodeType itemType,
                                         DbUiManager *uiManager);
};

#endif // USERCONTEXTMENUUTIL_H

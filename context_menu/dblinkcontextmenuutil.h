#ifndef DBLINKCONTEXTMENUUTIL_H
#define DBLINKCONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class DbLinkContextMenuUtil
{
public:
    DbLinkContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // DBLINKCONTEXTMENUUTIL_H

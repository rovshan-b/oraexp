#ifndef CONTEXTMENUUTIL_H
#define CONTEXTMENUUTIL_H

#include "navtree/dbtreemodel.h"

class QAction;

class ContextMenuUtil
{
public:
    ContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // CONTEXTMENUUTIL_H

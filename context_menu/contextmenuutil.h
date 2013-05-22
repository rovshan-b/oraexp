#ifndef CONTEXTMENUUTIL_H
#define CONTEXTMENUUTIL_H

#include <QDomElement>
#include "navtree/dbtreemodel.h"

class QAction;
class DbItemDynamicAction;

class ContextMenuUtil
{
public:
    ContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);

private:
    static QList<QAction*> getActionsForObjectFromConfiguration(const QString &schemaName,
                                               const QString &objectName,
                                               const DbTreeModel::DbTreeNodeType itemType,
                                               DbUiManager *uiManager);
    static QAction *actionFromElement(const QDomElement &e,
                                                  const QString &schemaName,
                                                  const QString &objectName,
                                                  const DbTreeModel::DbTreeNodeType itemType,
                                                  DbUiManager *uiManager);
};

#endif // CONTEXTMENUUTIL_H

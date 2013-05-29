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
                                        const QString &parentObjectName,
                                               const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager);

private:
    static QList<QAction*> getActionsForObjectFromConfiguration(const QString &schemaName,
                                               const QString &objectName,
                                               const QString &parentObjectName,
                                               const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager);
    static QList<QAction *> actionsFromElement(const QDomElement &element,
                                        const QString &schemaName,
                                        const QString &objectName,
                                        const QString &parentObjectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager, QObject *parent);
    static QAction *actionFromElement(const QDomElement &e,
                                                  const QString &schemaName,
                                                  const QString &objectName,
                                                  const QString &parentObjectName,
                                                  const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager, QObject *parent);
};

#endif // CONTEXTMENUUTIL_H

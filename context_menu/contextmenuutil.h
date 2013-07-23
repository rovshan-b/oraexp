#ifndef CONTEXTMENUUTIL_H
#define CONTEXTMENUUTIL_H

#include <QDomElement>
#include "navtree/dbtreemodel.h"

class QAction;
class ContextMenuActionCreator;

class ContextMenuUtil
{
public:
    ContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const QString &parentObjectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);

    static QList<QAction*> getActionsFromFile(const QString &baseName,
                                              DbUiManager *uiManager,
                                              QObject *resultListener,
                                              const QHash<QString, QString> &properties = QHash<QString,QString>());

private:
    static bool readFromFile(const QString &baseName, QDomElement &rootElement);

    static QList<QAction*> getActionsForObjectFromConfiguration(const QString &schemaName,
                                               const QString &objectName,
                                               const QString &parentObjectName,
                                               const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager);
    static QList<QAction *> actionsFromElement(const QDomElement &element, ContextMenuActionCreator *actionCreator, QObject *parent, QObject *resultListener);
    static QAction *actionFromElement(const QDomElement &e, ContextMenuActionCreator *actionCreator, QObject *parent, QObject *resultListener);
};

#endif // CONTEXTMENUUTIL_H

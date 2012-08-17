#include "contextmenuutil.h"
#include "tablecontextmenuutil.h"
#include "viewcontextmenuutil.h"
#include "procedurecontextmenuutil.h"

ContextMenuUtil::ContextMenuUtil()
{
}

QList<QAction *> ContextMenuUtil::getActionsForObject(const QString &schemaName,
                                                      const QString &objectName,
                                                      const DbTreeModel::DbTreeNodeType itemType,
                                                      DbUiManager *uiManager)
{
    QList<QAction*> results;

    switch(itemType){
    case DbTreeModel::Tables:
    case DbTreeModel::Table:
        results = TableContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Views:
    case DbTreeModel::View:
        results = ViewContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Procedures:
    case DbTreeModel::Procedure:
        results = ProcedureContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    default:
        break;
    }

    return results;
}

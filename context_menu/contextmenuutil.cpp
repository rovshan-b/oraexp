#include "contextmenuutil.h"
#include "tablecontextmenuutil.h"
#include "viewcontextmenuutil.h"
#include "packagecontextmenuutil.h"
#include "procedurecontextmenuutil.h"
#include "functioncontextmenuutil.h"
#include "triggercontextmenuutil.h"
#include "typecontextmenuutil.h"

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
    case DbTreeModel::Packages:
    case DbTreeModel::Package:
    case DbTreeModel::PackageSpec:
    case DbTreeModel::PackageBody:
        results = PackageContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Procedures:
    case DbTreeModel::Procedure:
        results = ProcedureContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Functions:
    case DbTreeModel::Function:
        results = FunctionContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::SchemaTriggers:
    case DbTreeModel::Triggers:
    case DbTreeModel::Trigger:
        results = TriggerContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Types:
    case DbTreeModel::Type:
    case DbTreeModel::TypeSpec:
    case DbTreeModel::TypeBody:
        results = TypeContextMenuUtil::getActionsForObject(schemaName, objectName, itemType, uiManager);
        break;
    default:
        break;
    }

    return results;
}

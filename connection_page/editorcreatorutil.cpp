#include "editorcreatorutil.h"
#include "connection_page/table_creator/tablecreator.h"
#include "connection_page/code_creator/codecreator.h"
#include "connection_page/code_creator/paircodecreator.h"
#include "connection_page/table_info/tableinfoviewer.h"
#include "connection_page/sequence_creator/sequencecreator.h"

EditorCreatorUtil::EditorCreatorUtil()
{
}

ConnectionPageTab *EditorCreatorUtil::createEditor(const QString &schemaName,
                                         const QString &objectName,
                                         const DbTreeModel::DbTreeNodeType itemType,
                                         DbUiManager *uiManager)
{
    ConnectionPageTab *editor=0;

    switch(itemType){
    case DbTreeModel::Tables:
    case DbTreeModel::Table:
        editor = new TableCreator(schemaName, objectName, uiManager);
        break;
    case DbTreeModel::Views:
    case DbTreeModel::View:
        editor = new CodeCreator(schemaName, objectName, DbTreeModel::View, uiManager);
        break;
    case DbTreeModel::Packages:
    case DbTreeModel::Package:
    case DbTreeModel::PackageSpec:
        editor = new PairCodeCreator(schemaName, objectName, DbTreeModel::Package, uiManager);
        break;
    case DbTreeModel::PackageBody:
        editor = new PairCodeCreator(schemaName, objectName, DbTreeModel::PackageBody, uiManager);
        break;
    case DbTreeModel::Procedures:
    case DbTreeModel::Procedure:
        editor = new CodeCreator(schemaName, objectName, DbTreeModel::Procedure, uiManager);
        break;
    case DbTreeModel::Functions:
    case DbTreeModel::Function:
        editor = new CodeCreator(schemaName, objectName, DbTreeModel::Function, uiManager);
        break;
    case DbTreeModel::SchemaTriggers:
    case DbTreeModel::Triggers:
    case DbTreeModel::Trigger:
        editor = new CodeCreator(schemaName, objectName, DbTreeModel::Trigger, uiManager);
        break;
    case DbTreeModel::Sequences:
    case DbTreeModel::Sequence:
        editor = new SequenceCreator(schemaName, objectName, uiManager);
        break;
    case DbTreeModel::Types:
    case DbTreeModel::Type:
    case DbTreeModel::TypeSpec:
        editor = new PairCodeCreator(schemaName, objectName, DbTreeModel::Type, uiManager);
        break;
    case DbTreeModel::TypeBody:
        editor = new PairCodeCreator(schemaName, objectName, DbTreeModel::TypeBody, uiManager);
        break;
    default:
        break;
    }

    Q_ASSERT(editor);

    return editor;
}

ConnectionPageTab *EditorCreatorUtil::createViewer(const QString &schemaName,
                                         const QString &objectName,
                                         const DbTreeModel::DbTreeNodeType itemType,
                                         DbUiManager *uiManager)
{
    ConnectionPageTab *viewer=0;

    switch(itemType){
    case DbTreeModel::Table:
        viewer = new TableInfoViewer(schemaName, objectName, uiManager);
        break;
    default:
        break;
    }

    Q_ASSERT(viewer);

    return viewer;
}

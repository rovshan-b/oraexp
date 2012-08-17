#include "editorcreatorutil.h"
#include "connection_page/table_creator/tablecreator.h"
#include "connection_page/code_creator/codecreator.h"
#include "connection_page/table_info/tableinfoviewer.h"

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
    case DbTreeModel::Procedures:
    case DbTreeModel::Procedure:
        editor = new CodeCreator(schemaName, objectName, DbTreeModel::Procedure, uiManager);
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

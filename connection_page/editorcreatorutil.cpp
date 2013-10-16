#include "editorcreatorutil.h"
#include "connection_page/table_creator/tablecreator.h"
#include "connection_page/code_creator/codecreator.h"
#include "connection_page/code_creator/paircodecreator.h"
#include "connection_page/sequence_creator/sequencecreator.h"
#include "connection_page/synonym_creator/synonymcreator.h"
#include "connection_page/dblink_creator/dblinkcreator.h"
#include "connection_page/user_creator/usercreator.h"
#include "connection_page/table_viewer/tableviewer.h"
#include "connection_page/view_viewer/viewviewer.h"
#include "connection_page/code_viewer/codeviewer.h"
#include "connection_page/code_viewer/paircodeviewer.h"
#include "connection_page/index_viewer/indexviewer.h"
#include "connection_page/sequence_viewer/sequenceviewer.h"
#include "connection_page/synonym_viewer/synonymviewer.h"
#include "connection_page/dblink_viewer/dblinkviewer.h"
#include "connection_page/user_viewer/userviewer.h"
#include "connection_page/generic_viewer/genericobjectviewer.h"

EditorCreatorUtil::EditorCreatorUtil()
{
}

ConnectionPageTab *EditorCreatorUtil::createEditor(const QString &schemaName,
                                         const QString &objectName,
                                         const DbTreeModel::DbTreeNodeType itemType,
                                         DbUiManager *uiManager,
                                         DbObjectCreator::CreatorMode creatorMode)
{
    ConnectionPageTab *editor=0;

    switch(itemType){
    case DbTreeModel::Tables:
    case DbTreeModel::Table:
        editor = new TableCreator(schemaName, objectName, uiManager, creatorMode);
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
        editor = new SequenceCreator(schemaName, objectName, uiManager, creatorMode);
        break;
    case DbTreeModel::Types:
    case DbTreeModel::Type:
    case DbTreeModel::TypeSpec:
        editor = new PairCodeCreator(schemaName, objectName, DbTreeModel::Type, uiManager);
        break;
    case DbTreeModel::TypeBody:
        editor = new PairCodeCreator(schemaName, objectName, DbTreeModel::TypeBody, uiManager);
        break;
    case DbTreeModel::Synonyms:
    case DbTreeModel::Synonym:
        editor = new SynonymCreator(schemaName, objectName, uiManager, creatorMode);
        break;
    case DbTreeModel::DatabaseLinks:
    //case DbTreeModel::PublicDatabaseLinks:
    case DbTreeModel::DatabaseLink:
    case DbTreeModel::PublicDatabaseLink:
        editor = new DbLinkCreator(schemaName, objectName, uiManager, creatorMode);
        break;
    case DbTreeModel::AllSchemas:
    case DbTreeModel::Schema:
        editor = new UserCreator(schemaName, objectName, uiManager, creatorMode);
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
        viewer = new TableViewer(schemaName, objectName, uiManager);
        break;
    case DbTreeModel::View:
        viewer = new ViewViewer(schemaName, objectName, uiManager);
        break;
    case DbTreeModel::Procedure:
    case DbTreeModel::Function:
    case DbTreeModel::Trigger:
        viewer = new CodeViewer(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::PackageSpec:
    case DbTreeModel::PackageBody:
    case DbTreeModel::TypeSpec:
    case DbTreeModel::TypeBody:
        viewer = new PairCodeViewer(schemaName, objectName, itemType, uiManager);
        break;
    case DbTreeModel::Index:
        viewer = new IndexViewer(schemaName, objectName, uiManager);
        break;
    case DbTreeModel::Sequence:
        viewer = new SequenceViewer(schemaName, objectName, uiManager);
        break;
    case DbTreeModel::Synonym:
        viewer = new SynonymViewer(schemaName, objectName, uiManager);
        break;
    case DbTreeModel::DatabaseLink:
    case DbTreeModel::PublicDatabaseLink:
        viewer = new DbLinkViewer(schemaName, objectName, uiManager);
        break;
    case DbTreeModel::Schema:
        viewer = new UserViewer(schemaName, objectName, uiManager);
        break;
    case DbTreeModel::Unknown:
        viewer = new GenericObjectViewer(schemaName, objectName, uiManager);
        break;
    default:
        break;
    }

    Q_ASSERT(viewer);

    return viewer;
}

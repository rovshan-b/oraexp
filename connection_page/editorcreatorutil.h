#ifndef EDITORCREATORUTIL_H
#define EDITORCREATORUTIL_H

#include "navtree/dbtreemodel.h"
#include "db_object_creator/dbobjectcreator.h"

class ConnectionPageTab;

class EditorCreatorUtil
{
public:
    EditorCreatorUtil();

    static ConnectionPageTab *createEditor(const QString &schemaName,
                                 const QString &objectName,
                                 const DbTreeModel::DbTreeNodeType itemType,
                                 DbUiManager *uiManager,
                                 DbObjectCreator::CreatorMode creatorMode);

    static ConnectionPageTab *createViewer(const QString &schemaName,
                                 const QString &objectName,
                                 const DbTreeModel::DbTreeNodeType itemType,
                                 DbUiManager *uiManager);
};

#endif // EDITORCREATORUTIL_H

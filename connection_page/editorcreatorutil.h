#ifndef EDITORCREATORUTIL_H
#define EDITORCREATORUTIL_H

#include "navtree/dbtreemodel.h"

class ConnectionPageTab;

class EditorCreatorUtil
{
public:
    EditorCreatorUtil();

    static ConnectionPageTab *createEditor(const QString &schemaName,
                                 const QString &objectName,
                                 const DbTreeModel::DbTreeNodeType itemType,
                                 DbUiManager *uiManager, bool editMode);

    static ConnectionPageTab *createViewer(const QString &schemaName,
                                 const QString &objectName,
                                 const DbTreeModel::DbTreeNodeType itemType,
                                 DbUiManager *uiManager);
};

#endif // EDITORCREATORUTIL_H

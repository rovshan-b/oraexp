#ifndef PROCEDURECONTEXTMENUUTIL_H
#define PROCEDURECONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class ProcedureContextMenuUtil
{
public:
    ProcedureContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // PROCEDURECONTEXTMENUUTIL_H

#ifndef SEQUENCECONTEXTMENUUTIL_H
#define SEQUENCECONTEXTMENUUTIL_H

#include "beans/dbitemaction.h"

class SequenceContextMenuUtil
{
public:
    SequenceContextMenuUtil();

    static QList<QAction*> getActionsForObject(const QString &schemaName,
                                        const QString &objectName,
                                        const DbTreeModel::DbTreeNodeType itemType,
                                        DbUiManager *uiManager);
};

#endif // SEQUENCECONTEXTMENUUTIL_H

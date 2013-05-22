#ifndef DBITEMDYNAMICACTION_H
#define DBITEMDYNAMICACTION_H

#include "dbitemaction.h"

class DbItemDynamicAction : public DbItemAction
{
    Q_OBJECT
public:
    explicit DbItemDynamicAction(const QIcon &icon, const QString &text,
                                 const QString &schemaName,
                                 const QString &objectName,
                                 const DbTreeModel::DbTreeNodeType itemType,
                                 const QObject *receiver=0,
                                 const char *slotName=0,
                                 QObject *parent = 0);

    virtual ~DbItemDynamicAction();
    
};

#endif // DBITEMDYNAMICACTION_H

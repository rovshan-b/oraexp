#include "sequencecontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

SequenceContextMenuUtil::SequenceContextMenuUtil()
{
}

QList<QAction *> SequenceContextMenuUtil::getActionsForObject(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::Sequences){
        QAction *createSequenceAction=new DbItemAction(IconUtil::getIcon("sequence_add"), QObject::tr("Create sequence"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createCreator()));
        results.append(createSequenceAction);
    }else if(itemType==DbTreeModel::Sequence){
        //view
        QAction *viewSequenceAction=new DbItemAction(IconUtil::getIcon("sequence_view"), QObject::tr("View"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createViewer()));

        //alter
        DbItemAction *alterSequenceAction=new DbItemAction(IconUtil::getIcon("sequence_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        alterSequenceAction->isDefault = true;
        results.append(viewSequenceAction);
        results.append(alterSequenceAction);
    }

    return results;
}

#include "procedurecontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

ProcedureContextMenuUtil::ProcedureContextMenuUtil()
{
}

QList<QAction *> ProcedureContextMenuUtil::getActionsForObject(const QString &schemaName,
                                                               const QString &objectName,
                                                               const DbTreeModel::DbTreeNodeType itemType,
                                                               DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::Procedure){
        //alter
        QAction *alterProcedureAction=new DbItemAction(IconUtil::getIcon("procedure"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        results.append(alterProcedureAction);
    }

    return results;
}

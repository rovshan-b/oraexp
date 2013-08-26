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

    if(itemType==DbTreeModel::Procedures){
        QAction *createProcedureAction=new DbItemAction(IconUtil::getIcon("procedure_add"), QObject::tr("Create procedure"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createCreator()));
        results.append(createProcedureAction);
    }else if(itemType==DbTreeModel::Procedure){
        //view
        QAction *viewProcedureAction=new DbItemAction(IconUtil::getIcon("procedure_view"), QObject::tr("View"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createViewer()));

        //alter
        DbItemAction *alterProcedureAction=new DbItemAction(IconUtil::getIcon("procedure_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        alterProcedureAction->isDefault = true;

        results.append(viewProcedureAction);
        results.append(alterProcedureAction);
    }

    return results;
}

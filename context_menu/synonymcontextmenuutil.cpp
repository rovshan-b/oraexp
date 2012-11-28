#include "synonymcontextmenuutil.h"
#include "util/iconutil.h"
#include "connection_page/dbuimanager.h"

SynonymContextMenuUtil::SynonymContextMenuUtil()
{
}

QList<QAction *> SynonymContextMenuUtil::getActionsForObject(const QString &schemaName, const QString &objectName, const DbTreeModel::DbTreeNodeType itemType, DbUiManager *uiManager)
{
    QList<QAction *> results;

    if(itemType==DbTreeModel::Synonyms){
        DbItemAction *createSynonymAction=new DbItemAction(IconUtil::getIcon("synonym_add"), QObject::tr("Create synonym"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createCreator()));
        if(schemaName=="PUBLIC"){
            createSynonymAction->properties["public"]="yes";
        }
        results.append(createSynonymAction);
    }else if(itemType==DbTreeModel::Synonym){
        //view
        QAction *viewSynonymAction=new DbItemAction(IconUtil::getIcon("synonym_view"), QObject::tr("View"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createViewer()));

        //alter
        QAction *alterSynonymAction=new DbItemAction(IconUtil::getIcon("synonym_alter"), QObject::tr("Alter"),
                                                       schemaName, objectName, itemType,
                                                       uiManager, SLOT(createEditor()));
        results.append(viewSynonymAction);
        results.append(alterSynonymAction);
    }

    return results;
}

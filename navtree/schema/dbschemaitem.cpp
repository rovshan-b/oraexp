#include "dbschemaitem.h"
#include "../table/dbtableparenttreeitem.h"
#include "../view/dbviewparentitem.h"
#include "../package/dbpackageparentitem.h"
#include "../procedure/dbprocedureparentitem.h"
#include "../function/dbfunctionparentitem.h"
#include "../trigger/dbtriggerparentitem.h"
#include "../index/dbindexparentitem.h"
#include "../sequence/dbsequenceparentitem.h"
#include "../type/dbtypeparentitem.h"
#include "../synonym/dbsynonymparentitem.h"
#include "../dblink/dbdblinkparentitem.h"
#include "dballschemasitem.h"
#include "util/dbutil.h"

DbSchemaItem::DbSchemaItem(const QString &itemText, const QString &itemName,
                           DbTreeModel *model, DbTreeItem *parent, bool isRootItem) :
        DbTreeItem(itemText, itemName, model, parent),
        isRootItem(isRootItem)
{
}

QString DbSchemaItem::iconName() const
{
    return "user";
}

QList<DbTreeItem*> DbSchemaItem::populateChildren()
{
    QList<DbTreeItem*> children;

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Tables)){
        children.append(new DbTableParentTreeItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::Tables), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Views)){
        children.append(new DbViewParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::Views), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Packages)){
        children.append(new DbPackageParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::Packages), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Procedures)){
        children.append(new DbProcedureParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::Procedures), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Functions)){
        children.append(new DbFunctionParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::Functions), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::SchemaTriggers)){
        children.append(new DbTriggerParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::Triggers), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::SchemaIndexes)){
        children.append(new DbIndexParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::Indexes), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Sequences)){
         children.append(new DbSequenceParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::Sequences), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Types)){
        children.append(new DbTypeParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::Types), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Synonyms)){
        children.append(new DbSynonymParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::Synonyms), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::PublicSynonyms)){
        DbTreeItem *publicSynonymsItem = new DbSynonymParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::PublicSynonyms), "", getModel(), this);
        publicSynonymsItem->setSchemaName("PUBLIC");
        children.append(publicSynonymsItem);
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::DatabaseLinks)){
        children.append(new DbDbLinkParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::DatabaseLinks), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::PublicDatabaseLinks)){
        DbTreeItem *publicDbLinksItem = new DbDbLinkParentItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::PublicDatabaseLinks), "", getModel(), this);
        publicDbLinksItem->setSchemaName("PUBLIC");
        children.append(publicDbLinksItem);
    }

    if(isRootItem && nodeTypesToDisplay.testFlag(DbTreeModel::AllSchemas)){
        children.append(new DbAllSchemasItem(DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::AllSchemas), "", getModel(), this, nodeTypesToDisplay));
    }

    return children;
}

bool DbSchemaItem::displayChildCount() const
{
    return false;
}

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
        children.append(new DbTableParentTreeItem(QObject::tr("Tables"), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Views)){
        children.append(new DbViewParentItem(QObject::tr("Views"), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Packages)){
        children.append(new DbPackageParentItem(QObject::tr("Packages"), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Procedures)){
        children.append(new DbProcedureParentItem(QObject::tr("Procedures"), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Functions)){
        children.append(new DbFunctionParentItem(QObject::tr("Functions"), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::SchemaTriggers)){
        children.append(new DbTriggerParentItem(QObject::tr("Triggers"), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::SchemaIndexes)){
        children.append(new DbIndexParentItem(QObject::tr("Indexes"), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Sequences)){
         children.append(new DbSequenceParentItem(QObject::tr("Sequences"), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Types)){
        children.append(new DbTypeParentItem(QObject::tr("Types"), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::Synonyms)){
        children.append(new DbSynonymParentItem(QObject::tr("Synonyms"), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::PublicSynonyms)){
        DbTreeItem *publicSynonymsItem = new DbSynonymParentItem(QObject::tr("Public synonyms"), "", getModel(), this);
        publicSynonymsItem->setSchemaName("PUBLIC");
        children.append(publicSynonymsItem);
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::DatabaseLinks)){
        children.append(new DbDbLinkParentItem(QObject::tr("Database links"), "", getModel(), this));
    }

    if(nodeTypesToDisplay.testFlag(DbTreeModel::PublicDatabaseLinks)){
        DbTreeItem *publicDbLinksItem = new DbDbLinkParentItem(QObject::tr("Public database links"), "", getModel(), this);
        publicDbLinksItem->setSchemaName("PUBLIC");
        children.append(publicDbLinksItem);
    }

    if(isRootItem && nodeTypesToDisplay.testFlag(DbTreeModel::AllSchemas)){
        children.append(new DbAllSchemasItem(QObject::tr("All schemas"), "", getModel(), this, nodeTypesToDisplay));
    }

    return children;
}

bool DbSchemaItem::displayChildCount() const
{
    return false;
}

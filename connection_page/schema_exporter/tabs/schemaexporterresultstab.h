#ifndef SCHEMAEXPORTERRESULTSTAB_H
#define SCHEMAEXPORTERRESULTSTAB_H

#include "connection_page/db_object_comparer/tabs/dbobjectcomparerresultstab.h"
#include "navtree/dbtreemodel.h"

class SchemaExporterResultsTab : public DbObjectComparerResultsTab
{
    Q_OBJECT
public:
    explicit SchemaExporterResultsTab(DbUiManager *uiManager, QWidget *parent = 0);
    
public slots:
    void objectExportStarted(DbTreeModel::DbTreeNodeType parentNodeType, const QString &objectName);
    void objectExportCompleted(DbTreeModel::DbTreeNodeType parentNodeType, const QString &objectName);

    virtual void setCurrentSchema(const QString &currentSchemaName);
    
};

#endif // SCHEMAEXPORTERRESULTSTAB_H

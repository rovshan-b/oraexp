#ifndef SCHEMAEXPORTERCOMPARETAB_H
#define SCHEMAEXPORTERCOMPARETAB_H

#include "connection_page/db_object_comparer/tabs/dbobjectcomparercomparetab.h"

class SchemaExporterCompareTab : public DbObjectComparerCompareTab
{
    Q_OBJECT
public:
    explicit SchemaExporterCompareTab(DbUiManager *uiManager, QWidget *parent = 0);

    virtual bool needsTargetConnection() const;
    
protected:
    virtual DbTreeModel::DbTreeNodeTypes getNodeTypesToDisplay() const;
    virtual DbTreeModel::DbTreeNodeTypes getCheckableNodeTypes() const;
    
};

#endif // SCHEMAEXPORTERCOMPARETAB_H

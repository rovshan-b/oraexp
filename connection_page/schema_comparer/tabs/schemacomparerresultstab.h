#ifndef SCHEMACOMPARERRESULTSTAB_H
#define SCHEMACOMPARERRESULTSTAB_H

#include "connection_page/db_object_comparer/tabs/dbobjectcomparerresultstab.h"
#include "navtree/dbtreemodel.h"

class SchemaComparerResultsTab : public DbObjectComparerResultsTab
{
    Q_OBJECT
public:
    explicit SchemaComparerResultsTab(QWidget *parent);

public slots:
    void changedObjectDetected(const QString &objectName, DbTreeModel::DbTreeNodeType objectType, const QString &compareResult);
    
};

#endif // SCHEMACOMPARERRESULTSTAB_H

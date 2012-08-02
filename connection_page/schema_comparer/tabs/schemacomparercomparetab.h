#ifndef SCHEMACOMPARERCOMPARETAB_H
#define SCHEMACOMPARERCOMPARETAB_H

#include "connection_page/db_object_comparer/tabs/dbobjectcomparercomparetab.h"

class SchemaComparerCompareTab : public DbObjectComparerCompareTab
{
    Q_OBJECT
public:
    explicit SchemaComparerCompareTab(DbUiManager *uiManager, QWidget *parent);


};

#endif // SCHEMACOMPARERCOMPARETAB_H

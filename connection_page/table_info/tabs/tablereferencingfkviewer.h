#ifndef TABLEREFERENCINGFKVIEWER_H
#define TABLEREFERENCINGFKVIEWER_H

#include "../tableinfogenerictab.h"

class DataTable;

class TableReferencingFKViewer : public TableInfoGenericTab
{
    Q_OBJECT
public:
    TableReferencingFKViewer(QWidget *parent);

    virtual void setTableName(const QString &schemaName, const QString &tableName);
};
#endif // TABLEREFERENCINGFKVIEWER_H

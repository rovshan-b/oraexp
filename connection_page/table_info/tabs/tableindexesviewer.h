#ifndef TABLEINDEXESVIEWER_H
#define TABLEINDEXESVIEWER_H

#include "../tableinfogenerictab.h"

class TableIndexesViewer : public TableInfoGenericTab
{
    Q_OBJECT
public:
    explicit TableIndexesViewer(QWidget *parent = 0);

    virtual void setTableName(const QString &schemaName, const QString &tableName);

signals:

public slots:

};

#endif // TABLEINDEXESVIEWER_H

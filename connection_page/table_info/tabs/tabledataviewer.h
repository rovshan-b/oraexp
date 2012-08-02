#ifndef TABLEDATAVIEWER_H
#define TABLEDATAVIEWER_H

#include "../tableinfogenerictab.h"

class TableDataViewer : public TableInfoGenericTab
{
    Q_OBJECT
public:
    explicit TableDataViewer(QWidget *parent = 0);

    virtual QList<Param *> getQueryParams();

    virtual void setTableName(const QString &schemaName, const QString &tableName);

    virtual void createMainWidget(QLayout *);
};

#endif // TABLEDATAVIEWER_H

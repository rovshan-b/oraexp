#ifndef TABLEDATAVIEWER_H
#define TABLEDATAVIEWER_H

#include "../../db_object_viewer/dbobjectviewergenerictab.h"

class TableDataViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit TableDataViewer(QWidget *parent = 0);

    virtual QList<Param *> getQueryParams();

    virtual void setObjectName(const QString &schemaName, const QString &tableName);

    virtual void createMainWidget(QLayout *);
};

#endif // TABLEDATAVIEWER_H

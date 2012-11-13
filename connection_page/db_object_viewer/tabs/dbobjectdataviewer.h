#ifndef DBOBJECTDATAVIEWER_H
#define DBOBJECTDATAVIEWER_H

#include "../../db_object_viewer/dbobjectviewergenerictab.h"

class DbObjectDataViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit DbObjectDataViewer(QWidget *parent = 0);

    virtual QList<Param *> getQueryParams();

    virtual void setObjectName(const QString &schemaName, const QString &tableName);

    virtual void createMainWidget(QLayout *);
};

#endif // DBOBJECTDATAVIEWER_H

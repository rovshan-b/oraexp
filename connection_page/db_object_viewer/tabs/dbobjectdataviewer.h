#ifndef DBOBJECTDATAVIEWER_H
#define DBOBJECTDATAVIEWER_H

#include "../../db_object_viewer/dbobjectviewergenerictab.h"

class DbObjectDataViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit DbObjectDataViewer(DbUiManager *uiManager, QWidget *parent = 0);

    virtual QList<Param *> getQueryParams();

    virtual void setObjectName(const QString &schemaName, const QString &objectName, DbTreeModel::DbTreeNodeType itemType);

    virtual void createMainWidget(QLayout *);

    virtual QList<QAction*> getSpecificToolbarButtons();

private slots:
    void addRecord();
    void deleteRecord();

    void commit();
    void reset();

    void showDml();

    void setColumnDelegates();
};

#endif // DBOBJECTDATAVIEWER_H

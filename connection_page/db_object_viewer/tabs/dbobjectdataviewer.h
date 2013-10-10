#ifndef DBOBJECTDATAVIEWER_H
#define DBOBJECTDATAVIEWER_H

#include "../../db_object_viewer/dbobjectviewergenerictab.h"
#include "connectivity/fetchresult.h"

class DataTableEditController;

class DbObjectDataViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit DbObjectDataViewer(DbUiManager *uiManager, QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual QList<Param *> getQueryParams();

    virtual void setObjectName(const QString &schemaName, const QString &objectName, DbTreeModel::DbTreeNodeType itemType);

    virtual void createMainWidget(QLayout *);

    virtual QList<QAction*> getSpecificToolbarButtons();

private slots:
    void filter();
    void sort(int colIx);

    void asyncQueryError(const OciException &ex);
private:
    DataTableEditController *editController;

    QString baseQuery;
    QString whereClause;
    QString orderBy;

    QString commitDml;

    void rebuildQuery();
    QString getCommitDml() const;
};

#endif // DBOBJECTDATAVIEWER_H

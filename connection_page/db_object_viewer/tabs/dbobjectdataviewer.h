#ifndef DBOBJECTDATAVIEWER_H
#define DBOBJECTDATAVIEWER_H

#include "../../db_object_viewer/dbobjectviewergenerictab.h"
#include "connectivity/fetchresult.h"

class DataTableEditController;
class ClickableLabel;

class DbObjectDataViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit DbObjectDataViewer(DbUiManager *uiManager, QWidget *parent = 0);

    virtual void setQueryScheduler(IQueryScheduler *queryScheduler);

    virtual QList<Param *> getQueryParams();

    virtual void setObjectName(const QString &schemaName, const QString &objectName, DbTreeModel::DbTreeNodeType itemType);

    virtual void createMainWidget(QLayout *);

    virtual QList<QAction*> getSpecificToolbarButtons(QToolBar *toolbar);
    virtual QList<QWidget*> getSpecificToolbarWidgets(QToolBar *toolbar);

private slots:
    void filter();
    void sort(int colIx);
    void countRows();

    void asyncQueryError(const OciException &ex);
private:
    DataTableEditController *editController;
    ClickableLabel *filterLabel;

    QString baseQuery;
    QString whereClause;
    QString orderBy;

    QString commitDml;

    bool isEditableResultset;

    void rebuildQuery();
    void appendWhereClause(QString &appendTo);
    QString getCommitDml() const;

    void displayFilterText();
};

#endif // DBOBJECTDATAVIEWER_H

#ifndef DBOBJECTDDLVIEWER_H
#define DBOBJECTDDLVIEWER_H

#include <QWidget>
#include "../../db_object_viewer/dbobjectviewergenerictab.h"
#include "connectivity/fetchresult.h"
#include "beans/objectddloptions.h"

class CodeEditor;

class DbObjectDdlViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit DbObjectDdlViewer(DbTreeModel::DbTreeNodeType itemType, QWidget *parent = 0);

    virtual void loadData();

    virtual void createMainWidget(QLayout *);

protected:
    virtual void addSpecificToolbarButtons();

private slots:
    void ddlQueryCompleted(const QueryResult &result);
    void ddlFetched(const FetchResult &fetchResult);
    void fetchCompleted(const QString &);

    void showDdlOptions();

private:
    CodeEditor *editor;

    ObjectDdlOptions ddlOptions;

    DbTreeModel::DbTreeNodeType itemType;

};

#endif // DBOBJECTDDLVIEWER_H

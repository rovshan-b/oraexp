#ifndef TABLEDDLVIEWER_H
#define TABLEDDLVIEWER_H

#include <QWidget>
#include "../../db_object_viewer/dbobjectviewergenerictab.h"
#include "connectivity/fetchresult.h"
#include "beans/objectddloptions.h"

class CodeEditor;

class TableDdlViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit TableDdlViewer(QWidget *parent = 0);

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

};

#endif // TABLEDDLVIEWER_H

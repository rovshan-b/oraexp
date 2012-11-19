#ifndef DBOBJECTDDLVIEWER_H
#define DBOBJECTDDLVIEWER_H

#include <QWidget>
#include "../../db_object_viewer/dbobjectviewergenerictab.h"
#include "connectivity/fetchresult.h"
#include "beans/objectddloptions.h"

class CodeEditorAndSearchPaneWidget;

class DbObjectDdlViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit DbObjectDdlViewer(bool addSettingsButton, QWidget *parent = 0);

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
    CodeEditorAndSearchPaneWidget *editor;

    ObjectDdlOptions ddlOptions;

    bool addSettingsButton;

};

#endif // DBOBJECTDDLVIEWER_H
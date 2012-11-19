#ifndef DBOBJECTTRIGGERSVIEWER_H
#define DBOBJECTTRIGGERSVIEWER_H

#include "../../db_object_viewer/dbobjectviewergenerictab.h"
#include "connectivity/fetchresult.h"

class CodeEditor;

class DbObjectTriggersViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit DbObjectTriggersViewer(QWidget *parent = 0);

    virtual void createMainWidget(QLayout *layout);

private slots:
    void currentRowChanged();
    void sourceFetchQueryCompleted(const QueryResult &result);
    void sourceRecordAvailable(const FetchResult &fetchResult);
    void triggerBodyFetched(const QString &);

protected:
    virtual void clearInfo();
    virtual QList<Param*> getQueryParams();

private:
    CodeEditor *editor;
    bool sourceRetrieveError;

};

#endif // DBOBJECTTRIGGERSVIEWER_H

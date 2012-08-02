#ifndef TABLETRIGGERSVIEWER_H
#define TABLETRIGGERSVIEWER_H

#include "../tableinfogenerictab.h"
#include "connectivity/fetchresult.h"

class CodeEditor;

class TableTriggersViewer : public TableInfoGenericTab
{
    Q_OBJECT
public:
    explicit TableTriggersViewer(QWidget *parent = 0);

    virtual void createMainWidget(QLayout *layout);

private slots:
    void currentRowChanged();
    void sourceFetchQueryCompleted(const QueryResult &result);
    void sourceRecordAvailable(const FetchResult &fetchResult);
    void triggerBodyFetched(const QString &);

protected:
    virtual void clearInfo();

private:
    CodeEditor *editor;
    bool sourceRetrieveError;

};

#endif // TABLETRIGGERSVIEWER_H

#ifndef TABLEPARTITIONSVIEWER_H
#define TABLEPARTITIONSVIEWER_H

#include <QWidget>
#include "../../db_object_viewer/dbobjectviewergenerictab.h"
#include "connectivity/fetchresult.h"

class DataTable;
class QLabel;
class SubTabWidget;

class TablePartitionsViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit TablePartitionsViewer(QWidget *parent = 0);

    virtual void createMainWidget(QLayout *layout);

    virtual void loadData();

protected:
    virtual void clearInfo();

private slots:
    void currentRowChanged();
    void partitioningInfoLoaded(const QueryResult &result);
    void partitionInfoFetched(const FetchResult &fetchResult);
    void partitionInfoFetchCompleted(const QString &);
    void partitionListFirstFetchCompleted();

private:
    DataTable *dtSubpartitions;
    DataTable *dtSubpartitionTemplate;
    QLabel *partitioningInfoLabel;
    bool isPartitioned;
    bool complexPartitioning;
    SubTabWidget *bottomPaneTab;

};

#endif // TABLEPARTITIONSVIEWER_H

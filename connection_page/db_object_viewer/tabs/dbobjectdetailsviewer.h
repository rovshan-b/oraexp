#ifndef DBOBJECTDETAILSVIEWER_H
#define DBOBJECTDETAILSVIEWER_H

#include "../dbobjectviewergenerictab.h"

#include "connectivity/fetchresult.h"

class QScrollArea;
class QLabel;

class DbObjectDetailsViewer : public DbObjectViewerGenericTab
{
    Q_OBJECT
public:
    explicit DbObjectDetailsViewer(const QString &queryName, DbUiManager *uiManager, QWidget *parent = 0);

    virtual void createMainWidget(QLayout *layout);

    virtual void loadData();
    
private slots:
    void objectDetailsQueryCompleted(const QueryResult &result);
    void objectDetailsRecordAvailable(const FetchResult &fetchResult);
    void objectDetailsFetched(const QString &);

private:
    QScrollArea *scrollArea;
    QList<QLabel*> labels;

    void createLabels(const FetchResult &result);
    void setLabelTexts(const FetchResult &result);
    
};

#endif // DBOBJECTDETAILSVIEWER_H

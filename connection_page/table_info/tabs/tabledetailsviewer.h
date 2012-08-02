#ifndef TABLEDETAILSVIEWER_H
#define TABLEDETAILSVIEWER_H

#include <QWidget>
#include "../tableinfogenerictab.h"
#include "connectivity/fetchresult.h"

class QScrollArea;
class QLabel;

class TableDetailsViewer : public TableInfoGenericTab
{
    Q_OBJECT
public:
    explicit TableDetailsViewer(QWidget *parent = 0);

    virtual void createMainWidget(QLayout *layout);

    virtual void loadData();

private slots:
    void tableDetailsQueryCompleted(const QueryResult &result);
    void tableDetailsRecordAvailable(const FetchResult &fetchResult);
    void tableDetailsFetched(const QString &);

private:
    QScrollArea *scrollArea;
    QList<QLabel*> labels;

    void createLabels(const FetchResult &result);
    void setLabelTexts(const FetchResult &result);

};

#endif // TABLEDETAILSVIEWER_H

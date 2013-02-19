#ifndef SCROLLABLERESULTSETTABLEMODEL_H
#define SCROLLABLERESULTSETTABLEMODEL_H

#include "resultsettablemodel.h"

class ScrollableResultsetTableModel : public ResultsetTableModel
{
public:
    ScrollableResultsetTableModel(IQueryScheduler *queryScheduler, Resultset *rs, QObject *parent,
                                  const QHash<int, StatementDesc*> &dynamicQueries=QHash<int, StatementDesc*>(),
                                  const QHash<QString, QString> &iconColumns=QHash<QString, QString>(),
                                  bool humanizeColumnNames=false);

    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

protected slots:
    virtual void recordsFetched(const QList<QStringList> &records);

protected:
    enum FetchMode
    {
        FetchMore,
        FetchCurrent
    };

    virtual void createFetcherThread();
    virtual bool deleteResultsetOnFetchComplete() const;

private:
    int currentOffset; //zero based index of first fetched record index currently in memory
    mutable int currentOffsetInProgress;
    FetchMode currentFetchMode;
};

#endif // SCROLLABLERESULTSETTABLEMODEL_H

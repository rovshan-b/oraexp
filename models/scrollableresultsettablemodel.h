#ifndef SCROLLABLERESULTSETTABLEMODEL_H
#define SCROLLABLERESULTSETTABLEMODEL_H

#include "resultsettablemodel.h"

class QTableView;

class ScrollableResultsetTableModel : public ResultsetTableModel
{
    Q_OBJECT
public:
    ScrollableResultsetTableModel(IQueryScheduler *queryScheduler, Resultset *rs, QObject *parent,
                                  const QHash<int, StatementDesc*> &dynamicQueries=QHash<int, StatementDesc*>(),
                                  const QHash<QString, QString> &iconColumns=QHash<QString, QString>(),
                                  bool humanizeColumnNames=false);

    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

public slots:
    bool fetchData();

protected slots:
    virtual void recordsFetched(const QList<QStringList> &records);
    virtual void fetchComplete();
    void existingRecordsFetched(const QList<QStringList> &records);

protected:
    virtual void deleteFetcherThread();

    virtual bool deleteResultsetOnFetchComplete() const;

private slots:
    void scrolled(int value);

private:
    int currentOffset; //zero based index of first fetched record index currently in memory

    void shrinkModelData(bool fromTop);

    QTableView *getParentView();
    void getFirstLastVisibleIndexes(QModelIndex &first, QModelIndex &last);
    int getVisibleRowCount();
    int getCacheSize();
    bool isInCache(int row) const;
};

#endif // SCROLLABLERESULTSETTABLEMODEL_H

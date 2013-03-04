#ifndef RESULTSETTABLEMODEL_H
#define RESULTSETTABLEMODEL_H

#include <QAbstractTableModel>
#include "connectivity/ociexception.h"

class IQueryScheduler;
class Resultset;
class RecordFetcherThread;
class StatementDesc;

class ResultsetTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ResultsetTableModel(IQueryScheduler *queryScheduler, Resultset *rs, QObject *parent,
                                 const QHash<int, StatementDesc*> &dynamicQueries=QHash<int, StatementDesc*>(),
                                 const QHash<QString, QString> &iconColumns=QHash<QString, QString>(),
                                 bool humanizeColumnNames=false);
    virtual ~ResultsetTableModel();

    virtual int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual int	columnCount ( const QModelIndex & parent = QModelIndex() ) const;

    virtual bool canFetchMore ( const QModelIndex & parent ) const;
    virtual void fetchMore ( const QModelIndex & parent );

    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

    void setFetchSize(int fetchSize);

    QList< QStringList > getModelData() const;
    Resultset *getResultset() const;
    bool isAllDataFetched() const;
    void setAllDataFetched();
    void setFetchInProgress(bool inProgress);
signals:
    void firstFetchCompleted();

protected:
    bool isValidIndex(const QModelIndex & index, int role) const;

    bool readyToFetch() const;
    void startFetcherThread();
    virtual void deleteFetcherThread();

    IQueryScheduler *queryScheduler;
    Resultset *rs;
    QHash<int, StatementDesc*> dynamicQueries;
    QHash<unsigned int, unsigned int> iconColumns;

    bool humanizeColumnNames;

    QList< QStringList > modelData;
    int fetchedRowCount;

    int rsColumnCount;
    bool allDataFetched;
    QHash<QString, unsigned int> columnIndexes;

    RecordFetcherThread *fetcherThread;

    bool fetchInProgress;

    virtual bool deleteResultsetOnFetchComplete() const;

    QVariant getColumnIcon(const QList<QString> &oneRow, unsigned int colIx) const;

protected slots:
    virtual void recordsFetched(const QList<QStringList> &records);
    virtual void fetchComplete();
    void fetchError(const OciException &ex);

private:
    bool hasIconColumn;

    bool firstFetchDone;

    int fetchSize;

    void deleteResultset();

};

#endif // RESULTSETTABLEMODEL_H

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
signals:
    void firstFetchCompleted();

private slots:
    void recordsFetched(const QList<QStringList> &records);
    void fetchComplete();
    void fetchError(const OciException &ex);

private:
    IQueryScheduler *queryScheduler;
    Resultset *rs;
    QHash<int, StatementDesc*> dynamicQueries;
    QHash<unsigned int, unsigned int> iconColumns;

    bool humanizeColumnNames;

    QList< QList<QString> > modelData;
    int fetchedRowCount;

    int rsColumnCount;
    volatile bool allDataFetched;
    QHash<QString, unsigned int> columnIndexes;

    bool hasIconColumn;

    bool firstFetchDone;

    RecordFetcherThread *fetcherThread;
    void startFetcherThread();
    void deleteFetcherThread();

    bool fetchInProgress;

    QVariant getColumnIcon(const QList<QString> &oneRow, unsigned int colIx) const;

    void deleteResultset();

};

#endif // RESULTSETTABLEMODEL_H

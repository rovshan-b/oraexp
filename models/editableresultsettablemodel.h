#ifndef EDITABLERESULTSETTABLEMODEL_H
#define EDITABLERESULTSETTABLEMODEL_H

#include "resultsettablemodel.h"
#include <QStringList>

class EditableResultsetTableModel : public ResultsetTableModel
{
    Q_OBJECT
public:
    explicit EditableResultsetTableModel(IQueryScheduler *queryScheduler, Resultset *rs, QObject *parent,
                                         const QHash<int, StatementDesc*> &dynamicQueries=QHash<int, StatementDesc*>(),
                                         const QHash<QString, QString> &iconColumns=QHash<QString, QString>(),
                                         bool humanizeColumnNames=false);

    virtual int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;

    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
    virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual QVariant data ( const QModelIndex & ix, int role = Qt::DisplayRole ) const;
    virtual bool setData ( const QModelIndex & ix, const QVariant & value, int role = Qt::EditRole );

    bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );

    void resetChanges();
    void markRowAsDeleted(int row, bool mark = true);

    QString generateDmlAsString(const QString &schema, const QString &table) const;
    QMap<int, QString> generateDml(const QString &schema, const QString &table) const;
    
protected:
    //first hash: row, inner hash
    //inner hash: column, value
    QMap< int, QMap<int, QString> > changedData;
    QList< int > deletedRows;
    QList<QStringList> insertedRows;

    bool isChanged(int row, int column) const;
    
};

#endif // EDITABLERESULTSETTABLEMODEL_H

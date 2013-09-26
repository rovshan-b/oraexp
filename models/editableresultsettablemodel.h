#ifndef EDITABLERESULTSETTABLEMODEL_H
#define EDITABLERESULTSETTABLEMODEL_H

#include "resultsettablemodel.h"

class EditableResultsetTableModel : public ResultsetTableModel
{
    Q_OBJECT
public:
    explicit EditableResultsetTableModel(IQueryScheduler *queryScheduler, Resultset *rs, QObject *parent,
                                         const QHash<int, StatementDesc*> &dynamicQueries=QHash<int, StatementDesc*>(),
                                         const QHash<QString, QString> &iconColumns=QHash<QString, QString>(),
                                         bool humanizeColumnNames=false);

    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
    //virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    
protected:
    //first hash: row, inner hash
    //inner hash: column, value
    QHash< int, QHash<int, QString> > changedData;
    QList< int > deletedRows;

    bool isChanged(int row, int column) const;
    
};

#endif // EDITABLERESULTSETTABLEMODEL_H

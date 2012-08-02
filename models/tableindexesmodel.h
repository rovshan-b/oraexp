#ifndef TABLEINDEXESMODEL_H
#define TABLEINDEXESMODEL_H

#include "genericeditabletablemodel.h"
#include "enums.h"
#include "beans/indexinfo.h"

class TableIndexesModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit TableIndexesModel(const QStringList &headerTitles, QObject *parent);

    enum ColumnNames
    {
        IndexOwner,
        IndexName,
        IndexType,
        IndexColumns,
        IndexCompress,
        IndexPrefixLength,
        IndexParallel,
        IndexParallelDegree,
        IndexReverse,
        IndexStorage,
        IndexPartitioningType,
        IndexPartitionDefinition
    };

    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    QVariant data ( const QModelIndex & index, int role) const;

    QVariant getParallelEnableText(int parallelEnabledIx) const;

    virtual QString isRowDataCorrect(int rowIx) const;

    IndexInfo itemInfoFromModelRow(int row) const;
};

#endif // TABLEINDEXESMODEL_H

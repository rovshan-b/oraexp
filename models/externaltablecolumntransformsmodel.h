#ifndef EXTERNALTABLECOLUMNTRANSFORMSMODEL_H
#define EXTERNALTABLECOLUMNTRANSFORMSMODEL_H

#include "genericeditabletablemodel.h"

class ExternalTableColumnTransformsModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit ExternalTableColumnTransformsModel(const QStringList &headerTitles, QObject *parent);

    enum ColumnOrder
    {
        TransformColumn,
        TransformType,
        TransformFieldName,
        TransformLobType,
        TransformDirectories,
        TransformCharacterset
    };

    virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;
    virtual QVariant data ( const QModelIndex & index, int role) const;

private:
    bool isColumnEnabled(const QModelIndex &index) const;
};

#endif // EXTERNALTABLECOLUMNTRANSFORMSMODEL_H

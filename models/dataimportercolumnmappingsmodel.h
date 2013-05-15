#ifndef DATAIMPORTERCOLUMNMAPPINGSMODEL_H
#define DATAIMPORTERCOLUMNMAPPINGSMODEL_H

#include "genericeditabletablemodel.h"

class DataImporterColumnMappingsModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit DataImporterColumnMappingsModel(
            const QStringList &headerTitles,
            QObject *parent);

    enum ColumnOrder
    {
        ColumnName,
        FileField,
        ColumnFormat
    };

    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;

    void setColumnDataType(int row, const QString &dataType);
    QString getColumnDataType(int row) const;

    void setDateFormat(int row, const QStringList &dates);
    
private:
    bool isFormatFieldEnabled(int row) const;

    
};

#endif // DATAIMPORTERCOLUMNMAPPINGSMODEL_H

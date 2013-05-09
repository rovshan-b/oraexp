#ifndef DATAIMPORTERCOLUMNMAPPINGSMODEL_H
#define DATAIMPORTERCOLUMNMAPPINGSMODEL_H

#include "genericeditabletablemodel.h"

class DataImporterColumnMappingsModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit DataImporterColumnMappingsModel(
            const QStringList &headerTitles,
            QObject *parent = 0);

    enum ColumnOrder
    {
        ColumnName,
        FileField,
        ColumnFormat
    };
    
signals:
    
public slots:
    
};

#endif // DATAIMPORTERCOLUMNMAPPINGSMODEL_H

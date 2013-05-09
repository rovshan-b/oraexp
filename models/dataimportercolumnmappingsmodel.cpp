#include "dataimportercolumnmappingsmodel.h"

DataImporterColumnMappingsModel::DataImporterColumnMappingsModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent)
{
}

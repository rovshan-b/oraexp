#include "dataimportercolumnmappingsmodel.h"
#include "util/dbutil.h"
#include "util/strutil.h"
#include "defines.h"
#include <QApplication>
#include <QPalette>

#define COLUMN_DATA_TYPE_ROLE (Qt::UserRole+100)

DataImporterColumnMappingsModel::DataImporterColumnMappingsModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent)
{
}

Qt::ItemFlags DataImporterColumnMappingsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags itemFlags = GenericEditableTableModel::flags(index);

    if(itemFlags == Qt::NoItemFlags){
        return itemFlags;
    }

    if(index.column() == ColumnFormat && !isFormatFieldEnabled(index.row())){
        itemFlags = Qt::NoItemFlags;
    }

    return itemFlags;
}

QVariant DataImporterColumnMappingsModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > rowCount()){
        return QVariant();
    }

    QVariant itemData=GenericEditableTableModel::data(index, role);

    if(index.column() == ColumnFormat && role==Qt::BackgroundRole && !isFormatFieldEnabled(index.row())){
        itemData = QApplication::palette().background();
    }

    return itemData;
}

void DataImporterColumnMappingsModel::setColumnDataType(int row, const QString &dataType)
{
    Q_ASSERT(row>=0 && row<rowCount());

    setData(index(row, ColumnName), dataType, COLUMN_DATA_TYPE_ROLE);
}

QString DataImporterColumnMappingsModel::getColumnDataType(int row) const
{
    Q_ASSERT(row>=0 && row<rowCount());

    return index(row, ColumnName).data(COLUMN_DATA_TYPE_ROLE).toString();
}

void DataImporterColumnMappingsModel::setDateFormat(int row, const QString &date)
{
    if(isFormatFieldEnabled(row)){
        QString dateFormat;
        QString dataType = getColumnDataType(row);

        if(DbUtil::isTimestampType(dataType)){
            OraExp::ColumnSubType timestampSubType=DbUtil::getTimestampSubType(dataType);
            if(timestampSubType==OraExp::CSTTimestampTz){
                dateFormat = DB_TZ_TIMESTAMP_FORMAT;
            }else{
                dateFormat = DB_TIMESTAMP_FORMAT;
            }
        }else if(DbUtil::isDateType(dataType)){
            dateFormat = detectDateFormat(date);
        }

        if(!dateFormat.isEmpty()){
            setData(index(row, ColumnFormat), dateFormat);
        }
    }
}

bool DataImporterColumnMappingsModel::isFormatFieldEnabled(int row) const
{
    QString dataType = getColumnDataType(row);
    if(!(DbUtil::isDateType(dataType) || DbUtil::isTimestampType(dataType))){
        return false;
    }

    return true;
}

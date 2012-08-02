#include "externaltablefieldsmodel.h"
#include <QtGui>

ExternalTableFieldsModel::ExternalTableFieldsModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent)
{
}

QVariant ExternalTableFieldsModel::data ( const QModelIndex & index, int role ) const
{
    if(role==Qt::BackgroundRole && index.column()>=FieldTerminatedBy && index.column()<FieldNullIf && !isColumnEnabled(index))
    {
        return QApplication::palette().background();
    }

    return GenericEditableTableModel::data(index, role);
}

Qt::ItemFlags ExternalTableFieldsModel::flags ( const QModelIndex & index ) const
{
    if(index.column()>=FieldTerminatedBy && index.column()<FieldNullIf && !isColumnEnabled(index))
    {
        return Qt::NoItemFlags;
    }

    return GenericEditableTableModel::flags(index);
}

bool ExternalTableFieldsModel::isColumnEnabled(const QModelIndex &index) const
{
    int column = index.column();

    QString dataType=index.model()->index(index.row(), FieldDataType).data().toString();

    if(column>=FieldTerminatedBy && column<FieldTrim && !dataType.isEmpty() &&
             (dataType.startsWith("CHAR") ||
              dataType.contains("INTEGER") ||
              dataType.contains("DECIMAL") ||
              dataType.contains("ZONED") ||
              dataType.contains("FLOAT"))){
        return true;
    }else if(column>=FieldTrim && !dataType.isEmpty() && dataType.startsWith("CHAR")){
        if(column==FieldDateMask){
            QString dateType=index.model()->index(index.row(), FieldDateType).data().toString();
            return !dateType.isEmpty() && !dateType.contains("INTERVAL") ;
        }else{
            return true;
        }
    }

    return false;
}

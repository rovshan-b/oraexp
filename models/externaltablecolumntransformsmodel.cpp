#include "externaltablecolumntransformsmodel.h"
#include <QtGui>

ExternalTableColumnTransformsModel::ExternalTableColumnTransformsModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent)
{
}

QVariant ExternalTableColumnTransformsModel::data ( const QModelIndex & index, int role ) const
{
    if(role==Qt::BackgroundRole && index.column()>=TransformFieldName && !isColumnEnabled(index))
    {
        return QApplication::palette().background();
    }

    return GenericEditableTableModel::data(index, role);
}

Qt::ItemFlags ExternalTableColumnTransformsModel::flags ( const QModelIndex & index ) const
{
    if(index.column()>=TransformFieldName && !isColumnEnabled(index))
    {
        return Qt::NoItemFlags;
    }

    return GenericEditableTableModel::flags(index);
}

bool ExternalTableColumnTransformsModel::isColumnEnabled(const QModelIndex &index) const
{
    int column = index.column();

    QString transformType=index.model()->index(index.row(), TransformType).data().toString();
    if(transformType.isEmpty()){
        return false;
    }

    if(column>=TransformLobType){
        return transformType==tr("LOBFILE");
    }else if(column>=TransformFieldName && (transformType==tr("CONCAT") || transformType==tr("CONSTANT") || transformType==tr("LOBFILE"))){
        return true;
    }

    return false;
}

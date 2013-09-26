#include "editableresultsettablemodel.h"
#include "beans/resultsetcolumnmetadata.h"
#include "util/iconutil.h"
#include <QDebug>

EditableResultsetTableModel::EditableResultsetTableModel(IQueryScheduler *queryScheduler, Resultset *rs, QObject *parent, const QHash<int, StatementDesc *> &dynamicQueries, const QHash<QString, QString> &iconColumns, bool humanizeColumnNames) :
    ResultsetTableModel(queryScheduler, rs, parent, dynamicQueries, iconColumns, humanizeColumnNames)
{
}

Qt::ItemFlags EditableResultsetTableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags result = ResultsetTableModel::flags(index);

    if(index.column() != columnMetadata->columnTitles.size()-1){
        result |= Qt::ItemIsEditable;
    }

    return result;
}

/*
QVariant EditableResultsetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal){
        return ResultsetTableModel::headerData(section, orientation, role);
    }

    if(role == Qt::DisplayRole){
        return section + 1;
    }else if(role == Qt::DecorationRole){
        if(deletedRows.contains(section)){
            return IconUtil::getIcon("delete_small");
        }else if(changedData.contains(section)){
            return IconUtil::getIcon("edit_small");
        }
    }

    return QVariant();
}*/

QVariant EditableResultsetTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }

    QVariant value = ResultsetTableModel::data(index, role);
    if(!value.isValid() && !role==Qt::DecorationRole){
        return value;
    }

    bool changed = isChanged(index.row(), index.column());

    if(changed){
        if(role == Qt::DisplayRole || role == Qt::EditRole){
            value = changedData[index.row()][index.column()];
        }else if(role == Qt::DecorationRole){
            value = IconUtil::getIcon("edit_small");
        }
    }

    if(role == Qt::DecorationRole && deletedRows.contains(index.row())){
        value = IconUtil::getIcon("delete_small");
    }

    return value;
}

bool EditableResultsetTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!isValidIndex(index, role)){
        return false;
    }

    bool changed = false;

    //original - value in database/resultset
    //current value - old version of changed value

    QString originalValue = modelData.at(index.row()).at(index.column());
    QString newValue = value.toString();

    bool sameAsOriginal = (newValue.compare(originalValue) == 0);

    //check if hash contains entry for changed row
    bool hasRow = changedData.contains(index.row());
    if(hasRow){
        //check if inner hash contains entry for changed column
        QHash<int, QString> &innerHash = changedData[index.row()];
        bool hasColumn = innerHash.contains(index.column());

        if(hasColumn){
            QString currentValue = innerHash[index.column()];
            if(newValue.compare(currentValue)==0){
                changed = false;
            }else{
                changed = true;

                if(sameAsOriginal){ //same as original value. so there is no need to keep this value in hash
                    innerHash.remove(index.column());
                    if(innerHash.isEmpty()){
                        changedData.remove(index.row());
                    }
                }else{
                    innerHash[index.column()] = newValue;
                }
            }
        }else{
            if(!sameAsOriginal){
                innerHash[index.column()] = newValue;
                changed = true;
            }
        }
    }else{
        if(!sameAsOriginal){
            QHash<int, QString> &innerHash=changedData[index.row()];
            innerHash[index.column()] = newValue;
            changed = true;
        }
    }

    if(changed){
        emit dataChanged(index, index);
    }

    return changed;
}

bool EditableResultsetTableModel::isChanged(int row, int column) const
{
    return changedData.contains(row) && changedData.value(row).contains(column);
}

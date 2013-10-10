#include "editableresultsettablemodel.h"
#include "beans/resultsetcolumnmetadata.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include "util/strutil.h"
#include <QtGui>

EditableResultsetTableModel::EditableResultsetTableModel(IQueryScheduler *queryScheduler, Resultset *rs, QObject *parent, const QHash<int, StatementDesc *> &dynamicQueries, const QHash<QString, QString> &iconColumns, bool humanizeColumnNames) :
    ResultsetTableModel(queryScheduler, rs, parent, dynamicQueries, iconColumns, humanizeColumnNames)
{
}

int EditableResultsetTableModel::rowCount(const QModelIndex &parent) const
{
    return ResultsetTableModel::rowCount(parent) + insertedRows.size();
}

Qt::ItemFlags EditableResultsetTableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid()){
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags result = ResultsetTableModel::flags(index);

    int row = index.row() - insertedRows.size();

    bool isDeleted = deletedRows.contains(row);

    if(index.column() != columnMetadata->getColumnCount()-1 && !isDeleted){
        result |= Qt::ItemIsEditable;
    }

    return result;
}

QVariant EditableResultsetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal){
        return ResultsetTableModel::headerData(section, orientation, role);
    }

    if(role == Qt::DisplayRole){
        return section + 1;
    }else if(role == Qt::DecorationRole){

        if(section < insertedRows.size()){
            return IconUtil::getIcon("add_small");
        }

        int row = section - insertedRows.size();

        if(deletedRows.contains(row)){
            return IconUtil::getIcon("delete_small");
        }else if(changedData.contains(row)){
            return IconUtil::getIcon("edit_small");
        }
    }else if(role == Qt::SizeHintRole){
        return QSize(qApp->fontMetrics().width(QString::number(section)) + 30, 0);
    }

    return QVariant();
}

QVariant EditableResultsetTableModel::data(const QModelIndex &ix, int role) const
{
    if(!ix.isValid()){
        return QVariant();
    }

    if(ix.row()<insertedRows.size()){
        if((role==Qt::DisplayRole || role==Qt::EditRole) && ix.column() < columnCount()){
            return insertedRows.at(ix.row()).at(ix.column());
        }else{
            return QVariant();
        }
    }

    QModelIndex newIndex = insertedRows.size() == 0 ? ix : index(ix.row() - insertedRows.size(), ix.column());

    QVariant value = ResultsetTableModel::data(newIndex, role);
    if(!value.isValid() && !role==Qt::DecorationRole){
        return value;
    }

    bool isDeleted = deletedRows.contains(newIndex.row());
    bool changed = !isDeleted && isChanged(newIndex.row(), newIndex.column());

    if(changed){
        if(role == Qt::DisplayRole || role == Qt::EditRole){
            value = changedData[newIndex.row()][newIndex.column()];
        }else if(role == Qt::DecorationRole){
            value = IconUtil::getIcon("edit_small");
        }
    }

    if(role == Qt::FontRole && isDeleted){
        QFont f=qApp->font("QTableView");
        f.setStrikeOut(true);
        value = f;
    }

    return value;
}

bool EditableResultsetTableModel::setData(const QModelIndex &ix, const QVariant &value, int role)
{
    if(!isValidIndex(ix, role)){
        return false;
    }

    if(ix.row()<insertedRows.size()){
        if(role==Qt::DisplayRole || role==Qt::EditRole){
            insertedRows[ix.row()][ix.column()] = value.toString();
            emit dataChanged(ix, ix);
            return true;
        }else{
            return false;
        }
    }

    QModelIndex newIndex = insertedRows.size() == 0 ? ix : index(ix.row() - insertedRows.size(), ix.column());

    bool changed = false;

    //original - value in database/resultset
    //current value - old version of changed value

    QString originalValue = modelData.at(newIndex.row()).at(newIndex.column());
    QString newValue = value.toString();

    bool sameAsOriginal = (newValue.compare(originalValue) == 0);

    //check if hash contains entry for changed row
    bool hasRow = changedData.contains(newIndex.row());
    if(hasRow){
        //check if inner hash contains entry for changed column
        QMap<int, QString> &innerHash = changedData[newIndex.row()];
        bool hasColumn = innerHash.contains(newIndex.column());

        if(hasColumn){
            QString currentValue = innerHash[newIndex.column()];
            if(newValue.compare(currentValue)==0){
                changed = false;
            }else{
                changed = true;

                if(sameAsOriginal){ //same as original value. so there is no need to keep this value in hash
                    innerHash.remove(newIndex.column());
                    if(innerHash.isEmpty()){
                        changedData.remove(newIndex.row());
                    }
                }else{
                    innerHash[newIndex.column()] = newValue;
                }
            }
        }else{
            if(!sameAsOriginal){
                innerHash[newIndex.column()] = newValue;
                changed = true;
            }
        }
    }else{
        if(!sameAsOriginal){
            QMap<int, QString> &innerHash=changedData[newIndex.row()];
            innerHash[newIndex.column()] = newValue;
            changed = true;
        }
    }

    if(changed){
        emit dataChanged(ix, ix);
    }

    return changed;
}

bool EditableResultsetTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(row);
    Q_ASSERT(count > 0);

    beginInsertRows(parent, insertedRows.size(), insertedRows.size()+count-1);

    for(int i=0; i<count; ++i){
        QStringList list;
        list.reserve(columnMetadata->getColumnCount());

        for(int k=0; k<columnMetadata->getColumnCount(); ++k){
            list.append("");
        }
        insertedRows.append(list);
    }

    endInsertRows();

    return true;
}

bool EditableResultsetTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_ASSERT(row < insertedRows.size());

    beginRemoveRows(parent, row, row+count-1);

    for(int i=0; i<count; ++i){
        insertedRows.removeAt(row);
    }

    endRemoveRows();

    return true;
}

void EditableResultsetTableModel::resetChanges()
{
    beginResetModel();

    insertedRows.clear();
    changedData.clear();
    deletedRows.clear();

    endResetModel();
}

void EditableResultsetTableModel::markRowAsDeleted(int row, bool mark)
{
    Q_ASSERT(row >= 0 && row < rowCount());

    if(row < insertedRows.size()){
        removeRows(row, 1);
        return;
    }

    if(mark){
        deletedRows.append(row-insertedRows.size());
    }else{
        deletedRows.removeOne(row-insertedRows.size());
    }

    emit dataChanged(index(row, 0), index(row, columnCount()-1));
}



bool EditableResultsetTableModel::isChanged(int row, int column) const
{
    return changedData.contains(row) && changedData.value(row).contains(column);
}

void EditableResultsetTableModel::escapeFieldValue(QString &fieldValue, ResultsetColumnMetadata *metadata, int colIx, bool guessDateFormat) const
{
    OraExp::ColumnDataType dataType = metadata->getColumnDataType(colIx);
    if(!fieldValue.isEmpty() &&
            (DbUtil::isDateType(dataType) || DbUtil::isTimestampType(dataType)) &&
            !fieldValue.at(0).isDigit()){
        //do nothing, keep value as is
    }else{
        DbUtil::escapeFieldValue(fieldValue, metadata, colIx, guessDateFormat); //colIx is 1 based
    }
}

QString EditableResultsetTableModel::generateDmlAsString(const QString &schema, const QString &table, const QString &dblink) const
{
    QString result;

    QMap<int, QString> dml = generateDml(schema, table, dblink);
    QMapIterator< int, QString > i(dml);

    while(i.hasNext()){
        i.next();

        if(!result.isEmpty()){
            result.append("\n");
        }

        result.append(i.value()).append(";");
    }

    return result;
}

QMap<int, QString> EditableResultsetTableModel::generateDml(const QString &schema, const QString &table, const QString &dblink) const
{
    QMap<int, QString> result;
    QString dml;
    QString fullTableName = schema.isEmpty() ? QString("\"%1\"").arg(table) : QString("\"%1\".\"%2\"").arg(schema, table);
    if(!dblink.isEmpty()){
        fullTableName.append("@").append(dblink);
    }

    QString fieldValue;

    QString colNames = joinEnclosed(columnMetadata->columnTitles, ", ", "\"", columnMetadata->columnTitles.size()-1);

    for(int i=0; i<insertedRows.size(); ++i){
        dml = QString("INSERT INTO %1 (%2) VALUES (").arg(fullTableName, colNames);
        for(int k=0; k<columnMetadata->columnTitles.size()-1; ++k){
            fieldValue = index(i, k).data().toString();
            escapeFieldValue(fieldValue, columnMetadata.data(), k+1, true); //colIx is 1 based

            if(k>0){
                dml.append(", ");
            }

            dml.append(fieldValue);
        }

        dml.append(")");

        result[i] = dml;
    }

    QMapIterator< int, QMap<int, QString> > i(changedData);
    while (i.hasNext()) {
        i.next();
        dml.clear();

        if(deletedRows.contains(i.key())){
            continue;
        }

        QMapIterator< int, QString > i2(i.value());
        while (i2.hasNext()) {
            i2.next();

            if(!dml.isEmpty()){
                dml.append(", ");
            }

            QString columnName = columnMetadata->columnTitles.at(i2.key());
            fieldValue = i2.value();
            escapeFieldValue(fieldValue, columnMetadata.data(), i2.key()+1, true); //colIx is 1 based
            dml.append(QString("\"%1\" = %2").arg(columnName, fieldValue));
        }

        Q_ASSERT(!dml.isEmpty());

        dml.prepend(QString("UPDATE %1 SET ").arg(fullTableName));

        int modelRow = i.key() + insertedRows.size();

        dml.append(QString(" WHERE ROWID = '%1'").arg(index(modelRow, columnCount()-1).data().toString()));

        result[modelRow] = dml;
    }

    for(int i=0; i<deletedRows.size(); ++i){
        int modelRow = deletedRows.at(i) + insertedRows.size();

        dml = QString("DELETE FROM %1 WHERE ROWID = '%2'").arg(fullTableName, index(modelRow, columnCount()-1).data().toString());
        result[modelRow] = dml;
    }

    return result;
}

int EditableResultsetTableModel::insertedRowCount() const
{
    return insertedRows.size();
}

bool EditableResultsetTableModel::hasChanges() const
{
    return !insertedRows.isEmpty() ||
            !deletedRows.isEmpty() ||
            !changedData.isEmpty();
}

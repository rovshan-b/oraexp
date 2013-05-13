#include "genericeditabletablemodel.h"
#include <iostream>
#include "util/iconutil.h"

#include <QtGui>

using namespace std;

#define ROW_MARKED_FOR_DELETION_ROLE (Qt::UserRole+10)
#define ROW_CHANGED_ROLE (Qt::UserRole+11)
#define ROW_NEEDS_RECREATION_ROLE (Qt::UserRole+12)
#define ROW_DATA_CORRECT_ROLE (Qt::UserRole+13)

GenericEditableTableModel::GenericEditableTableModel(const QStringList &headerTitles,
                                                     QObject *parent) :
    QAbstractTableModel(parent), headerTitles(headerTitles), frozenRowIx(-1), titleColumnIx(0)
{

}

int GenericEditableTableModel::rowCount ( const QModelIndex & ) const
{
    return modelData.size();
}

int GenericEditableTableModel::columnCount ( const QModelIndex & ) const
{
    return headerTitles.size();
}

QVariant GenericEditableTableModel::data ( const QModelIndex & index, int role) const
{
    if(!index.isValid()){
        return QVariant();
    }

    if(index.row()>=modelData.size() || index.row()<0){
        return QVariant();
    }

    if(index.column()>=headerTitles.size() || index.column()<0){
        return QVariant();
    }

    if(role==Qt::BackgroundRole){
        int disabledUntilRow;
        if(!isColumnEnabled(index.column(), disabledUntilRow)){
            if(index.row()<=disabledUntilRow){
                return QApplication::palette().background();
            }
        }
    }

    if(role==Qt::FontRole && isRowDeleted(index.row())){
        QFont f=qApp->font("QTableView");
        f.setStrikeOut(true);
        return f;
    }

    if(role==Qt::ForegroundRole && isRowDeleted(index.row())){
        return QBrush(Qt::gray);
    }

    bool roleDataFound;
    QVariant columnDataForRole = getDataWithoutChecks(index, role, roleDataFound);

    if(role==Qt::DisplayRole && !roleDataFound){
        //if no data exists for DisplayRole, check EditRole as well
        columnDataForRole = getDataWithoutChecks(index, Qt::EditRole, roleDataFound);
        if(columnLists.contains(index.column()) && columnDataForRole.type()==QVariant::Int){
            int itemIx = columnDataForRole.toInt();
            QStringList columnListData = columnLists.value(index.column());
            if(itemIx>=0 && itemIx<columnListData.size()){
                return columnListData.at(itemIx);
            }
        }
    }else if(role==Qt::DecorationRole && !roleDataFound){
        QString displayData = data(index, Qt::DisplayRole).toString();
        if(!displayData.isEmpty() && columnIcons.contains(index.column())){
            columnDataForRole = columnIcons.value(index.column());
        }

        if(!columnDataForRole.isValid() && columnIconLists.contains(index.column())){
            QVariant editRoleData = getDataWithoutChecks(index, Qt::EditRole, roleDataFound);
            if(editRoleData.isValid() && editRoleData.type()==QVariant::Int){
                int itemIx = editRoleData.toInt();
                QList<QPixmap> columnIconList = columnIconLists.value(index.column());
                if(itemIx>=0 && itemIx<columnIconList.size()){
                    return columnIconList.at(itemIx);
                }
            }
        }
    }

    return columnDataForRole;
}

QVariant GenericEditableTableModel::getDataWithoutChecks ( const QModelIndex & index, int role, bool &roleDataFound) const
{
    QList<QHash<int,QVariant> > oneRow=modelData.at(index.row());
    QHash<int,QVariant> columnData = oneRow.at(index.column());

    bool roleDataExists=columnData.contains(role);
    if(roleDataExists){
        roleDataFound=true;
        return columnData.value(role);
    }else{
        roleDataFound=false;
        return QVariant();
    }
}

QVariant GenericEditableTableModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole &&
            role!=Qt::ToolTipRole &&
            role!=Qt::ForegroundRole &&
            role!=Qt::DecorationRole){
        return QVariant();
    }

    if(orientation==Qt::Vertical && (section<0 || section>=rowCount())){
        return QVariant();
    }else if(orientation==Qt::Horizontal && (section<0 || section>=headerTitles.size())){
        return QVariant();
    }

    if(role==Qt::DisplayRole){
        if(orientation==Qt::Vertical){
            return section+1;
        }

        return headerTitles.at(section);
    }else if(role==Qt::ToolTipRole){
        if(orientation==Qt::Vertical){

            if(isRowDeleted(section)){
                return tr("Row marked for deletion");
            }

            QString toolTipMsg = isRowDataCorrect(section);
            if(!toolTipMsg.isEmpty()){
                return toolTipMsg;
            }

            if(rowNeedsRecreation(section)){
                return tr("Row will be dropped and recreated");
            }

            return tr("Row data is correct");
        }
    }else if(role==Qt::ForegroundRole){
        if(orientation==Qt::Vertical){

            if(isRowDeleted(section)){
                return QBrush(Qt::gray);
            }

            if(isRowDataCorrect(section).isEmpty()){
                return QBrush(Qt::darkGreen);
            }else{
                return QBrush(Qt::darkRed);
            }
        }
    }else if(role==Qt::DecorationRole){
        if(orientation==Qt::Vertical){

            if(isRowDeleted(section)){
                return IconUtil::getIcon("delete_small");
            }

            if(rowNeedsRecreation(section)){
                return IconUtil::getIcon("delete_add_small");
            }

            if(isRowChanged(section)){
                return IconUtil::getIcon("edit_small");
            }

            if(frozenRowIx>=section){
                return IconUtil::getIcon("frozen_item_small");
            }

            return IconUtil::getIcon("add_small");
        }
    }

    return QVariant();
}

bool GenericEditableTableModel::setHeaderData (int section, Qt::Orientation orientation, const QVariant & value, int /*role*/)
{
    if(section<0 || section>=headerTitles.count()){
        return false;
    }

    if(orientation!=Qt::Horizontal){
        return false;
    }

    if(!value.isValid()){
        return false;
    }

    QString valueAsString=value.toString();

    if(headerTitles.at(section)!=valueAsString){
        headerTitles.replace(section, value.toString());
        emit headerDataChanged(orientation, section, section);
        return true;
    }else{
        return false;
    }
}

bool GenericEditableTableModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    if(!index.isValid()){
        return false;
    }

    //if(!value.isValid()){
    //    return false;
    //}

    if(index.row()>=modelData.size() || index.row()<0){
        return false;
    }

    if(index.column()>=headerTitles.size() || index.column()<0){
        return false;
    }

    //cout << "setting data for row " << index.row() << ", column " <<
    //        index.column() << ", role " << role << ", new value " << value.toString().toStdString() << endl;

    modelData[index.row()][index.column()][role].setValue(value);

    emit(dataChanged(index, index));

    return true;
}

Qt::ItemFlags GenericEditableTableModel::flags ( const QModelIndex & index ) const
{
    if(index.row()>=modelData.size() || index.row()<0){
        return Qt::NoItemFlags;
    }

    if(index.column()>=headerTitles.size() || index.column()<0){
        return Qt::NoItemFlags;
    }

    int disabledUntilRow;
    if(!isColumnEnabled(index.column(), disabledUntilRow)){
        if(index.row()<=disabledUntilRow){
            //return Qt::NoItemFlags;
            return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        }
    }

    return (QAbstractTableModel::flags(index) | Qt::ItemIsEditable);
}

bool GenericEditableTableModel::insertRows ( int row, int count, const QModelIndex & parent)
{
    if(row<0 || row>rowCount()){
        return false;
    }

    beginInsertRows(parent, row, row+count-1);

    modelData.reserve(rowCount()+count);


    int colCount=columnCount();
    int currentRowCount=rowCount();


    for(int i=0; i<count; i++){
        QList<QHash<int,QVariant> > newCols;
        newCols.reserve(colCount);
        for(int j=0; j<colCount; j++){
            newCols.append(QHash<int,QVariant>());
        }
        if(row==currentRowCount){
            modelData.append(newCols);
        }else{
            modelData.insert(row+i, newCols);
        }
    }


    endInsertRows();

    return true;
}


bool GenericEditableTableModel::removeRows ( int row, int count, const QModelIndex & parent)
{
    if(row<0 || row>rowCount()-1){
        return false;
    }

    beginRemoveRows(parent, row, row+count-1);

    for(int i=row+count-1; i>=row; --i){
        modelData.removeAt(i);
    }

    endRemoveRows();

    return true;
}

bool GenericEditableTableModel::moveRowUp(int row)
{
    if(row<=0){
        return false;
    }

    QList<QHash<int,QVariant> > currentRowData=modelData.at(row);
    QList<QHash<int,QVariant> > otherRowData=modelData.at(row-1);

    modelData[row]=otherRowData;
    modelData[row-1]=currentRowData;

    emit dataChanged(index(row-1, 0), index(row, columnCount()-1));

    return true;
}

bool GenericEditableTableModel::moveRowDown(int row)
{
    if(row>=rowCount()-2){
        return false;
    }

    QList<QHash<int,QVariant> > currentRowData=modelData.at(row);
    QList<QHash<int,QVariant> > otherRowData=modelData.at(row+1);

    modelData[row]=otherRowData;
    modelData[row+1]=currentRowData;

    emit dataChanged(index(row, 0), index(row+1, columnCount()-1));

    return true;
}

void GenericEditableTableModel::setColumnEnabled(int section, bool enable, int disabledUntilRow)
{
    if(enable && disabledColumns.contains(section)){
        disabledColumns.remove(section);

        emit dataChanged(index(0, section), index(rowCount()-1, section));
    }else if(!enable){
        disabledColumns[section]=disabledUntilRow;

        emit dataChanged(index(0, section), index(rowCount()-1, section));
    }
}

bool GenericEditableTableModel::isColumnEnabled(int section, int &disabledUntilRow) const
{
    bool contains=disabledColumns.contains(section);
    if(contains){
        disabledUntilRow=disabledColumns.value(section);
    }

    return !contains;
}

void GenericEditableTableModel::ensureRowCount(int minRowCount)
{
    int rowCountToAppend=minRowCount-(this->rowCount());
    if(rowCountToAppend>0){
        this->insertRows(this->rowCount(), rowCountToAppend);
    }else if(rowCountToAppend<0){
        this->removeRows(minRowCount, qAbs(rowCountToAppend)-1);
    }

    if(this->rowCount()<1){
        this->insertRow(0);
    }
}

bool GenericEditableTableModel::boolVal(int rowIx, int role) const
{
    Q_ASSERT(rowIx>=0 && rowIx<rowCount());

    QVariant val=index(rowIx, 0).data(role);
    return val.isValid() ? val.toBool() : false;
}
void GenericEditableTableModel::setBoolVal(int rowIx, int role, bool value)
{
    Q_ASSERT(rowIx>=0 && rowIx<rowCount());

    setData(index(rowIx, 0), value, role);
}

void GenericEditableTableModel::markRowAsDeleted(int rowIx, bool deleted)
{
    if(rowIx<0 || rowIx>=rowCount()){
        return;
    }

    if(boolVal(rowIx, ROW_MARKED_FOR_DELETION_ROLE)!=deleted){
        setBoolVal(rowIx, ROW_MARKED_FOR_DELETION_ROLE, deleted);

        emit headerDataChanged(Qt::Vertical, rowIx, rowIx);
    }
}

void GenericEditableTableModel::freezeRow(int row, bool moveDisabledRowIndexes)
{
    this->frozenRowIx=row;

    if(moveDisabledRowIndexes){
        for(int i=0; i<disabledColumns.size(); ++i){
            disabledColumns[i]=row;
        }
    }
}

bool GenericEditableTableModel::isRowDeleted(int rowIx) const
{
    if(rowIx<0 || rowIx>=rowCount()){
        return false;
    }

    return boolVal(rowIx, ROW_MARKED_FOR_DELETION_ROLE);
}

bool GenericEditableTableModel::isRowChanged(int rowIx) const
{
    if(rowIx<0 || rowIx>=rowCount()){
        return false;
    }

    return boolVal(rowIx, ROW_CHANGED_ROLE);
}

void GenericEditableTableModel::setRowChanged(int rowIx, bool changed)
{
    if(rowIx<0 || rowIx>=rowCount()){
        return;
    }

    if(boolVal(rowIx, ROW_CHANGED_ROLE)!=changed){
        setBoolVal(rowIx, ROW_CHANGED_ROLE, changed);

        emit headerDataChanged(Qt::Vertical, rowIx, rowIx);
    }
}

bool GenericEditableTableModel::rowNeedsRecreation(int rowIx) const
{
    return boolVal(rowIx, ROW_NEEDS_RECREATION_ROLE);
}

void GenericEditableTableModel::setRowNeedsRecreation(int rowIx, bool needsRecreation)
{
    if(boolVal(rowIx, ROW_NEEDS_RECREATION_ROLE)!=needsRecreation){
        setBoolVal(rowIx, ROW_NEEDS_RECREATION_ROLE, needsRecreation);

        emit headerDataChanged(Qt::Vertical, rowIx, rowIx);
    }
}

void GenericEditableTableModel::addCopyOf(int row)
{
    int rCount=rowCount();
    Q_ASSERT(row>=0 && row<rCount);

    QList<QHash<int,QVariant> > newRow=modelData.at(row);

    int newRowPosition=rCount;
    for(int i=frozenRowIx+1; i<rCount; ++i){
        if(!isRowDataCorrect(i).isEmpty()){
            newRowPosition=i;
            break;
        }
    }

    beginInsertRows(QModelIndex(), newRowPosition, newRowPosition);
    modelData.insert(newRowPosition, newRow);
    endInsertRows();

    markRowAsDeleted(newRowPosition, false);
    setRowNeedsRecreation(newRowPosition, false);
    setRowChanged(newRowPosition, false);

    emit dataChanged(index(newRowPosition, 0), index(newRowPosition, columnCount()-1));
    emit headerDataChanged(Qt::Vertical, newRowPosition, newRowPosition);
}

void GenericEditableTableModel::removeIncorrectRows()
{
    int rCount=rowCount();
    for(int i=rCount-1; i>frozenRowIx; --i){
        if(!isRowDataCorrect(i).isEmpty()){
            removeRow(i);
        }
    }
}

void GenericEditableTableModel::setColumnIcon(int columnIx, const QPixmap &icon)
{
    columnIcons[columnIx]=icon;
}

QPixmap GenericEditableTableModel::getColumnIcon(int columnIx)
{
    return columnIcons.value(columnIx);
}

QStringList GenericEditableTableModel::getList(int columnIx) const
{
    return columnLists.value(columnIx);
}

QList<QPixmap> GenericEditableTableModel::getIconList(int columnIx) const
{
    return columnIconLists.value(columnIx);
}

void GenericEditableTableModel::setList(int columnIx, const QStringList &list)
{
    Q_ASSERT(columnIx>=0 && columnIx<columnCount());

    columnLists[columnIx] = list;
}

void GenericEditableTableModel::setIconList(int columnIx, const QList<QPixmap> &list)
{
    Q_ASSERT(columnIx>=0 && columnIx<columnCount());

    columnIconLists[columnIx] = list;
}

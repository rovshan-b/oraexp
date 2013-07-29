#include "connectionlistmodel.h"
#include "beans/dbconnectioninfo.h"
#include "util/iconutil.h"

ConnectionListModel::ConnectionListModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

ConnectionListModel::~ConnectionListModel()
{
    qDeleteAll(connectionList);
}

int ConnectionListModel::rowCount(const QModelIndex &) const
{
    return connectionList.size();
}

int ConnectionListModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant ConnectionListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role!=Qt::DisplayRole){
        return QAbstractItemModel::headerData(section, orientation, role);
    }

    if(orientation == Qt::Vertical){
        return section + 1;
    }

    QString result;

    switch(section){
    case 0:
        result = tr("Connection name");
        break;
    case 1:
        result = tr("Username");
        break;
    case 2:
        result = tr("Database");
        break;
    default:
        break;
    }

    return result;
}

QVariant ConnectionListModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int column = index.column();

    if(row < 0 || row >= connectionList.size()){
        return QVariant();
    }

    if(column < 0 || column >= columnCount()){
        return QVariant();
    }

    if(role == Qt::DisplayRole){
        switch(column){
        case 0:
            return connectionList.at(row)->title;
            break;
        case 1:
            return connectionList.at(row)->username;
            break;
        default:
            return connectionList.at(row)->connectionString;
            break;
        }
    }else if(role == Qt::DecorationRole && column == 0){
        return IconUtil::getEnvironmentIcon(connectionList.at(row)->environment);
    }

    return QVariant();
}

void ConnectionListModel::addConnection(DbConnectionInfo *connectionInfo)
{
    beginInsertRows(QModelIndex(), connectionList.size(), connectionList.size());

    connectionList.append(connectionInfo);

    endInsertRows();
}

void ConnectionListModel::deleteConnection(DbConnectionInfo *connectionInfo)
{
    Q_ASSERT(!connectionInfo->uuid.isEmpty());

    int ix = indexOf(connectionInfo);

    if(ix == -1){
        return;
    }

    beginRemoveRows(QModelIndex(), ix, ix);

    connectionList.removeAt(ix);

    delete connectionInfo;

    endRemoveRows();
}

void ConnectionListModel::connectionUpdated(DbConnectionInfo *connectionInfo)
{
    if(connectionInfo->uuid.isEmpty()){
        return;
    }

    int ix = indexOf(connectionInfo);

    if(ix == -1){
        return;
    }

    emit dataChanged(index(ix,0), index(ix,columnCount()-1));
    emit connectionModified(connectionInfo);
}

DbConnectionInfo *ConnectionListModel::connectionAt(int index) const
{
    if(index < 0 || index >= connectionList.size()){
        return 0;
    }

    return connectionList.at(index);
}

void ConnectionListModel::setConnectionList(QList<DbConnectionInfo *> connectionList)
{
    beginResetModel();

    qDeleteAll(this->connectionList);
    this->connectionList = connectionList;

    endResetModel();
}

const QList<DbConnectionInfo *> ConnectionListModel::getConnectionList() const
{
    return this->connectionList;
}

int ConnectionListModel::indexOf(DbConnectionInfo *connection) const
{
    return indexOf(connection->uuid);
}

int ConnectionListModel::indexOf(const QString &uuid) const
{
    for(int i=0; i<connectionList.size(); ++i){

        DbConnectionInfo *listItem = connectionList.at(i);

        if(listItem->uuid == uuid){
            return i;
        }
    }

    return -1;
}

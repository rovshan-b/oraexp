#ifndef CONNECTIONLISTMODEL_H
#define CONNECTIONLISTMODEL_H

#include <QAbstractTableModel>

class DbConnectionInfo;

class ConnectionListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ConnectionListModel(QObject *parent = 0);
    virtual ~ConnectionListModel();

    int rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void addConnection(DbConnectionInfo *connectionInfo);
    void deleteConnection(DbConnectionInfo *connectionInfo);
    void connectionUpdated(DbConnectionInfo *connectionInfo);

    DbConnectionInfo *connectionAt(int index) const;
    int indexOf(DbConnectionInfo *connection) const;
    int indexOf(const QString &uuid) const;

    void setConnectionList(QList<DbConnectionInfo*> connectionList);
    const QList<DbConnectionInfo*> getConnectionList() const;

signals:
    void connectionModified(DbConnectionInfo *connectionInfo);
    
private:
    QList<DbConnectionInfo*> connectionList;
    
};

#endif // CONNECTIONLISTMODEL_H

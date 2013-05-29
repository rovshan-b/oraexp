#ifndef GENERICEDITABLETABLEMODEL_H
#define GENERICEDITABLETABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QVariantList>
#include <QHash>
#include <QIcon>

class GenericEditableTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    explicit GenericEditableTableModel(
            const QStringList &headerTitles,
            QObject *parent);

    virtual int	rowCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual int	columnCount ( const QModelIndex & parent = QModelIndex() ) const;

    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    bool setHeaderData (int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);

    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );

    bool moveRowUp(int row);
    bool moveRowDown(int row);

    void setColumnEnabled(int section, bool enable, int disabledUntilRow=-1);
    bool isColumnEnabled(int section, int &disabledUntilRow) const;

    bool boolVal(int rowIx, int role) const;
    void setBoolVal(int rowIx, int role, bool value);

    virtual QString isRowDataCorrect(int) const {return "";}

    bool isRowChanged(int) const;
    void setRowChanged(int, bool);

    bool rowNeedsRecreation(int) const;
    void setRowNeedsRecreation(int, bool);

    void ensureRowCount(int minRowCount);

    bool isRowDeleted(int rowIx) const;
    void markRowAsDeleted(int rowIx, bool deleted=true);

    int getFrozenRow() const {return this->frozenRowIx;}
    bool isRowFrozen(int rowIx) const {return rowIx<=frozenRowIx;}
    void freezeRow(int row, bool moveDisabledRowIndexes=false);

    virtual void addCopyOf(int row);

    void removeIncorrectRows();

    int getTitleColumn()const {return this->titleColumnIx;}
    void setTitleColumn(int column){this->titleColumnIx=column;}

    void setColumnIcon(int columnIx, const QPixmap &icon);
    QPixmap getColumnIcon(int columnIx);

    QStringList getList(int columnIx) const;
    QList<QPixmap> getIconList(int columnIx) const;
    void setList(int columnIx, const QStringList &list);
    void setIconList(int columnIx, const QList<QPixmap> &list);

    QModelIndex findItem(int column, const QRegExp &regexp);
protected:
    QStringList headerTitles;

    QHash<int,int> disabledColumns;

    int frozenRowIx;

    void addRows(int row, int count);

private:
    QList< QList<QHash<int,QVariant> > > modelData;

    QVariant getDataWithoutChecks ( const QModelIndex & index, int role, bool &roleDataFound) const;

    int titleColumnIx;

    QHash<int,QPixmap> columnIcons;

    QHash<int, QStringList> columnLists;
    QHash<int, QList<QPixmap> > columnIconLists;

};

#endif // GENERICEDITABLETABLEMODEL_H

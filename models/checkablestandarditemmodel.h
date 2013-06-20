#ifndef CHECKABLESTANDARDITEMMODEL_H
#define CHECKABLESTANDARDITEMMODEL_H

#include <QStandardItemModel>

class CheckableStandardItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit CheckableStandardItemModel(int rows, int columns, QObject *parent);

    virtual bool setData (const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

public slots:
    void checkAll(bool check);
    void checkAll();
    void uncheckAll();
    
private:
    void updateCheckedState(QStandardItem *item);
};

#endif // CHECKABLESTANDARDITEMMODEL_H

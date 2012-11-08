#ifndef DBITEMLISTDELEGATE_H
#define DBITEMLISTDELEGATE_H

#include "comboboxdelegate.h"

class IQueryScheduler;

class DbItemListDelegate : public ComboBoxDelegate
{
    Q_OBJECT
public:
    explicit DbItemListDelegate(const QString &initialValue,
                                IQueryScheduler *queryScheduler,
                                const QString &queryName,
                                const QString &iconName,
                                QObject *parent,
                                bool appendRowIfLast=false);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                           const QModelIndex &index) const;

    void setInitialValue(const QString &initialValue){this->initialValue=initialValue;}

    void setIconColumn(int colNum);

protected:
    QString initialValue;
    IQueryScheduler *queryScheduler;
    QString queryName;
    QString iconName;
    bool appendRowIfLast;

    int iconColumn;
};

#endif // DBITEMLISTDELEGATE_H

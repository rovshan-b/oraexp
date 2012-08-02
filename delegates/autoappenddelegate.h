#ifndef AUTOAPPENDDELEGATE_H
#define AUTOAPPENDDELEGATE_H

#include <QStyledItemDelegate>

class AutoAppendDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit AutoAppendDelegate(QObject *parent);

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                           const QModelIndex &index) const;

};

#endif // AUTOAPPENDDELEGATE_H

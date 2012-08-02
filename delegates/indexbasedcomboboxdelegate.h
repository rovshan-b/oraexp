#ifndef INDEXBASEDCOMBOBOXDELEGATE_H
#define INDEXBASEDCOMBOBOXDELEGATE_H

#include "comboboxdelegate.h"

class IndexBasedComboBoxDelegate : public ComboBoxDelegate
{
    Q_OBJECT
public:
    explicit IndexBasedComboBoxDelegate(const QIcon &itemIcon, const QStringList &itemList, QObject *parent);

    //when using this constructor you must override createEditor method
    explicit IndexBasedComboBoxDelegate(QObject *parent = 0);

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                                        const QModelIndex &index) const;

};

#endif // INDEXBASEDCOMBOBOXDELEGATE_H

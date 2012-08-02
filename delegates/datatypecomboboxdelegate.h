#ifndef DATATYPECOMBOBOXDELEGATE_H
#define DATATYPECOMBOBOXDELEGATE_H

#include "comboboxdelegate.h"

class DataTypeComboBoxDelegate : public ComboBoxDelegate
{
    Q_OBJECT
public:
    explicit DataTypeComboBoxDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

private:
    mutable QAbstractItemModel *currentModel;
};

#endif // DATATYPECOMBOBOXDELEGATE_H

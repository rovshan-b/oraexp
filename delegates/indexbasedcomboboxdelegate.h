#ifndef INDEXBASEDCOMBOBOXDELEGATE_H
#define INDEXBASEDCOMBOBOXDELEGATE_H

#include "comboboxdelegate.h"

class IndexBasedComboBoxDelegate : public ComboBoxDelegate
{
    Q_OBJECT
public:
    explicit IndexBasedComboBoxDelegate(QObject *parent, int modelColumnIx);

    void setEditorData(QWidget *editor, const QModelIndex &index) const;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                                        const QModelIndex &index) const;

};

#endif // INDEXBASEDCOMBOBOXDELEGATE_H

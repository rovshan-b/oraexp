#ifndef ONDELETEACTIONDELEGATE_H
#define ONDELETEACTIONDELEGATE_H

#include "indexbasedcomboboxdelegate.h"

class OnDeleteActionDelegate : public IndexBasedComboBoxDelegate
{
    Q_OBJECT
public:
    explicit OnDeleteActionDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;
};

#endif // ONDELETEACTIONDELEGATE_H

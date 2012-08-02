#ifndef CONTRAINTTYPEDELEGATE_H
#define CONTRAINTTYPEDELEGATE_H

#include "indexbasedcomboboxdelegate.h"

class ContraintTypeDelegate : public IndexBasedComboBoxDelegate
{
    Q_OBJECT
public:
    explicit ContraintTypeDelegate(QObject *parent);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;
};

#endif // CONTRAINTTYPEDELEGATE_H

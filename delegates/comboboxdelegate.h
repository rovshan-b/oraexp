#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include "abstractcomboboxdelegate.h"

class ComboBoxDelegate : public AbstractComboBoxDelegate
{
    Q_OBJECT
public:
    explicit ComboBoxDelegate(QObject *parent, bool isEditable=true, const QIcon &itemIcon=QIcon(), const QStringList &itemList=QStringList(), bool convertToUpperCase=true);

    virtual void setList(const QStringList &itemList);
    virtual QStringList getList() const;

protected:
    QStringList itemList;

};

#endif // COMBOBOXDELEGATE_H

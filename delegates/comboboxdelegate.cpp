#include "comboboxdelegate.h"


ComboBoxDelegate::ComboBoxDelegate(QObject *parent, bool isEditable, const QIcon &itemIcon, const QStringList &itemList, bool convertToUpperCase) :
    AbstractComboBoxDelegate(parent, isEditable, itemIcon, convertToUpperCase), itemList(itemList)
{
}

QStringList ComboBoxDelegate::getList() const
{
    return this->itemList;
}

void ComboBoxDelegate::setList(const QStringList &itemList)
{
    this->itemList=itemList;
}

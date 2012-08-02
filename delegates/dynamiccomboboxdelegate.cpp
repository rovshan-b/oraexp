#include "dynamiccomboboxdelegate.h"
#include "interfaces/istringlistretriever.h"

DynamicComboBoxDelegate::DynamicComboBoxDelegate(QObject *parent, IStringListRetriever *columnNameRetriever, bool isEditable) :
    AbstractComboBoxDelegate(parent, isEditable), columnNameRetriever(columnNameRetriever)
{
}

QStringList DynamicComboBoxDelegate::getList() const
{
    return columnNameRetriever->getStringList();
}

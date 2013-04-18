#include "dynamiccomboboxdelegate.h"
#include "interfaces/istringlistretriever.h"

DynamicComboBoxDelegate::DynamicComboBoxDelegate(QObject *parent, IStringListRetriever *columnNameRetriever, bool isEditable) :
    ComboBoxDelegate(parent, isEditable), columnNameRetriever(columnNameRetriever)
{
}

QStringList DynamicComboBoxDelegate::getList(GenericEditableTableModel *model) const
{
    Q_UNUSED(model);

    return columnNameRetriever->getStringList();
}

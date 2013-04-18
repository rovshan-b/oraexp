#ifndef DYNAMICCOMBOBOXDELEGATE_H
#define DYNAMICCOMBOBOXDELEGATE_H

#include "comboboxdelegate.h"

class IStringListRetriever;

class DynamicComboBoxDelegate : public ComboBoxDelegate
{
    Q_OBJECT
public:
    explicit DynamicComboBoxDelegate(QObject *parent, IStringListRetriever *columnNameRetriever, bool isEditable);

protected:
    virtual QStringList getList(GenericEditableTableModel *model) const;

private:
    IStringListRetriever *columnNameRetriever;

};

#endif // DYNAMICCOMBOBOXDELEGATE_H

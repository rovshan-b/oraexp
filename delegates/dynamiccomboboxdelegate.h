#ifndef DYNAMICCOMBOBOXDELEGATE_H
#define DYNAMICCOMBOBOXDELEGATE_H

#include "abstractcomboboxdelegate.h"

class IStringListRetriever;

class DynamicComboBoxDelegate : public AbstractComboBoxDelegate
{
    Q_OBJECT
public:
    explicit DynamicComboBoxDelegate(QObject *parent, IStringListRetriever *columnNameRetriever, bool isEditable);

protected:
    virtual QStringList getList() const;

private:
    IStringListRetriever *columnNameRetriever;

};

#endif // DYNAMICCOMBOBOXDELEGATE_H

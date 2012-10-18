#ifndef TABLEGRANTSMODEL_H
#define TABLEGRANTSMODEL_H

#include "genericeditabletablemodel.h"
#include "beans/grantinfo.h"

class TableGrantsModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit TableGrantsModel(const QStringList &headerTitles, QObject *parent);

    enum ColumnOrder
    {
        GrantSchema,
        GrantSelect,
        GrantInsert,
        GrantUpdate,
        GrantDelete,
        GrantAlter,
        GrantIndex,
        GrantReferences,
        GrantDebug
    };

    QVariant data ( const QModelIndex & index, int role) const;

    virtual QString isRowDataCorrect(int rowIx) const;

    TableGrantInfo itemInfoFromModelRow(int row) const;

private:
    QVariant getGrantTypeText(int grantTypeIx) const;
};

#endif // TABLEGRANTSMODEL_H

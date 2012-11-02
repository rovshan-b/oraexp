#ifndef TABLESPACEQUOTASMODEL_H
#define TABLESPACEQUOTASMODEL_H

#include "genericeditabletablemodel.h"
#include "beans/tablespacequotainfo.h"

class TablespaceQuotasModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit TablespaceQuotasModel(const QStringList &headerTitles, QObject *parent);
    
    enum ColumnOrder
    {
        TablespaceName,
        Unlimited,
        Quota,
        Unit
    };

    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    QVariant data ( const QModelIndex & index, int role) const;

    virtual QString isRowDataCorrect(int rowIx) const;
    TablespaceQuotaInfo itemInfoFromModelRow(int row) const;

private:
    bool isUnlimited(int rowIx) const;
    
};

#endif // TABLESPACEQUOTASMODEL_H

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
        Quota
    };

    virtual QString isRowDataCorrect(int rowIx) const;
    TablespaceQuotaInfo itemInfoFromModelRow(int row) const;
    
};

#endif // TABLESPACEQUOTASMODEL_H

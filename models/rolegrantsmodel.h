#ifndef ROLEGRANTSMODEL_H
#define ROLEGRANTSMODEL_H

#include "genericeditabletablemodel.h"
#include "beans/rolegrantinfo.h"

class RoleGrantsModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit RoleGrantsModel(const QStringList &headerTitles, QObject *parent);
    
    enum ColumnOrder
    {
        RoleName,
        IsGrantable,
        IsDefault
    };

    virtual QString isRowDataCorrect(int rowIx) const;
    RoleGrantInfo itemInfoFromModelRow(int row) const;
    
};

#endif // ROLEGRANTSMODEL_H

#ifndef SYSPRIVGRANTSMODEL_H
#define SYSPRIVGRANTSMODEL_H

#include "genericeditabletablemodel.h"
#include "beans/sysprivgrantinfo.h"

class SysPrivGrantsModel : public GenericEditableTableModel
{
    Q_OBJECT
public:
    explicit SysPrivGrantsModel(const QStringList &headerTitles, QObject *parent);
    
    enum ColumnOrder
    {
        PrivName,
        IsGrantable
    };

    virtual QString isRowDataCorrect(int rowIx) const;
    SysPrivGrantInfo itemInfoFromModelRow(int row) const;
    
};

#endif // SYSPRIVGRANTSMODEL_H

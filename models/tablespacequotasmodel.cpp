#include "tablespacequotasmodel.h"
#include "util/dbutil.h"

TablespaceQuotasModel::TablespaceQuotasModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent)
{
}

QString TablespaceQuotasModel::isRowDataCorrect(int rowIx) const
{
    QString tablespaceName=index(rowIx, TablespaceName).data().toString().trimmed();
    QString quota=index(rowIx, Quota).data().toString().trimmed();

    if(tablespaceName.isEmpty()){
        return tr("Tablespace name and quota must be entered");
    }

    return "";
}

TablespaceQuotaInfo TablespaceQuotasModel::itemInfoFromModelRow(int row) const
{
    TablespaceQuotaInfo info;
    info.quotaId=-1;

    if(!isRowDataCorrect(row).isEmpty()){
        return info;
    }

    info.tablespaceName=index(row, TablespaceName).data().toString().trimmed();

    bool unlimited;
    qulonglong quota;
    OraExp::ExtentUnit units;
    DbUtil::parseExtentSize(index(row, Quota).data().toString().trimmed(),
                            &unlimited,
                            &quota,
                            &units);

    info.unlimitedQuota=unlimited;

    if(!unlimited){
        info.quota=quota;
        info.quotaUnit=units;
    }

    info.quotaId=row+1;

    info.markedForDeletion=isRowDeleted(row);

    return info;
}

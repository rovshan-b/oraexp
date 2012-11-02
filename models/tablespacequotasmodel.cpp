#include "tablespacequotasmodel.h"
#include "util/dbutil.h"
#include <QtGui>

TablespaceQuotasModel::TablespaceQuotasModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent)
{
}

Qt::ItemFlags TablespaceQuotasModel::flags(const QModelIndex &index) const
{
    if((index.column()==Quota || index.column()==Unit) && isUnlimited(index.row())){
        return Qt::NoItemFlags;
    }

    return GenericEditableTableModel::flags(index);
}

QVariant TablespaceQuotasModel::data(const QModelIndex &index, int role) const
{
    if((index.column()==Quota || index.column()==Unit) && role==Qt::BackgroundRole && isUnlimited(index.row())){
        return QApplication::palette().background();
    }

    return GenericEditableTableModel::data(index, role);
}

QString TablespaceQuotasModel::isRowDataCorrect(int rowIx) const
{
    QString tablespaceName=index(rowIx, TablespaceName).data().toString().trimmed();
    bool unlimitedQuota=index(rowIx, Unlimited).data(Qt::EditRole).toBool();
    QString quotaText=index(rowIx, Quota).data().toString();

    if(tablespaceName.isEmpty() || (!unlimitedQuota && quotaText.isEmpty())){
        return tr("Tablespace name and quota must be entered");
    }

    return "";
}

TablespaceQuotaInfo TablespaceQuotasModel::itemInfoFromModelRow(int row) const
{
    TablespaceQuotaInfo info;
    info.objId=-1;

    if(!isRowDataCorrect(row).isEmpty()){
        return info;
    }

    info.tablespaceName=index(row, TablespaceName).data().toString().trimmed();
    info.unlimitedQuota=index(row, Unlimited).data(Qt::EditRole).toBool();
    info.quota=index(row, Quota).data().toULongLong();
    info.quotaUnit=(OraExp::ExtentUnit)index(row, Unit).data(Qt::EditRole).toInt();

    info.objId=row+1;

    info.markedForDeletion=isRowDeleted(row);

    return info;
}

bool TablespaceQuotasModel::isUnlimited(int rowIx) const
{
    return index(rowIx, Unlimited).data(Qt::EditRole).toBool();
}

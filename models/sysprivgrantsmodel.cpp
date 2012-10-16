#include "sysprivgrantsmodel.h"
#include "util/iconutil.h"

SysPrivGrantsModel::SysPrivGrantsModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent)
{
    setColumnIcon(0, IconUtil::getIcon("grants"));
}

QString SysPrivGrantsModel::isRowDataCorrect(int rowIx) const
{
    QString privName=index(rowIx, PrivName).data().toString().trimmed();

    if(privName.isEmpty()){
        return tr("Privilege name must be entered");
    }

    return "";
}

SysPrivGrantInfo SysPrivGrantsModel::itemInfoFromModelRow(int row) const
{
    SysPrivGrantInfo info;
    info.privId=-1;

    if(!isRowDataCorrect(row).isEmpty()){
        return info;
    }

    info.privName=index(row, PrivName).data().toString().trimmed();
    info.isGrantable=index(row, IsGrantable).data(Qt::EditRole).toBool();

    info.privId=row+1;

    info.markedForDeletion=isRowDeleted(row);

    return info;
}

#include "rolegrantsmodel.h"
#include "util/iconutil.h"

RoleGrantsModel::RoleGrantsModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent)
{
    setColumnIcon(0, IconUtil::getIcon("users"));
}

QString RoleGrantsModel::isRowDataCorrect(int rowIx) const
{
    QString roleName=index(rowIx, RoleName).data().toString().trimmed();

    if(roleName.isEmpty()){
        return tr("Role name must be entered");
    }

    return "";
}

RoleGrantInfo RoleGrantsModel::itemInfoFromModelRow(int row) const
{
    RoleGrantInfo info;
    info.roleId=-1;

    if(!isRowDataCorrect(row).isEmpty()){
        return info;
    }

    info.roleName=index(row, RoleName).data().toString().trimmed();
    info.isGrantable=index(row, IsGrantable).data(Qt::EditRole).toBool();
    info.isDefault=index(row, IsDefault).data(Qt::EditRole).toBool();

    info.roleId=row+1;

    info.markedForDeletion=isRowDeleted(row);

    return info;
}

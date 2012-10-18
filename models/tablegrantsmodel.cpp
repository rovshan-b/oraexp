#include "tablegrantsmodel.h"
#include "util/iconutil.h"
#include "util/dbutil.h"

TableGrantsModel::TableGrantsModel(const QStringList &headerTitles, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent)
{

}

QVariant TableGrantsModel::data ( const QModelIndex & index, int role) const
{
    int column=index.column();

    if(column>=GrantSelect && column<=GrantDebug && role==Qt::DisplayRole){
        QVariant grantOption=index.data(Qt::EditRole);
        if(grantOption.isValid()){
            return getGrantTypeText(grantOption.toInt());
        }
    }else if(column==GrantSchema && role==Qt::DecorationRole){
        bool isDataValid=index.data().isValid();
        if(isDataValid){
            return IconUtil::getIcon("user");
        }
    }

    return GenericEditableTableModel::data(index, role);
}


QVariant TableGrantsModel::getGrantTypeText(int grantTypeIx) const
{
    QVariant value;
    switch(grantTypeIx)
    {
    case 0:
        value="";
        break;
    case 1:
        value=tr("GRANT");
        break;
    default:
        value=tr("WITH GRANT OPTION");
        break;
    }

    return value;
}

QString TableGrantsModel::isRowDataCorrect(int rowIx) const
{
    QString grantSchema=index(rowIx, TableGrantsModel::GrantSchema).data().toString().trimmed().toUpper();

    if(grantSchema.isEmpty()){
        return tr("Grantee must be entered");
    }

    //QStringList privilegeNames;
    //DbUtil::populatePrivilegeNames(privilegeNames);
    //for(int k=0; k<privilegeNames.size(); ++k){
    //    if(index(rowIx, k+1 /*first column is schema*/).data(Qt::EditRole).toInt()!=0){
    //        return "";
    //    }
    //}

    //return tr("Granted privilege must be entered");

    return "";
}

TableGrantInfo TableGrantsModel::itemInfoFromModelRow(int row) const
{
    TableGrantInfo inf;
    inf.grantId=-1;

    if(!isRowDataCorrect(row).isEmpty()){
        return inf;
    }

    inf.grantee=index(row, GrantSchema).data().toString().trimmed().toUpper();

    QStringList privilegeNames;
    DbUtil::populatePrivilegeNames(privilegeNames);

    QString privName;
    for(int i=0; i<privilegeNames.size(); ++i){
        privName=privilegeNames.at(i);
        inf.privileges[privName]=index(row, i+1 /*1st column is schema name*/).data(Qt::EditRole).toInt();
    }

    inf.grantId=row+1;

    inf.markedForDeletion=isRowDeleted(row);

    return inf;
}

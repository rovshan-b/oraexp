#include "objectgrantsmodel.h"
#include "util/iconutil.h"
#include "util/dbutil.h"

ObjectGrantsModel::ObjectGrantsModel(const QStringList &headerTitles,
                                     OraExp::GrantsEditorPerspective perspective,
                                     const QStringList &privilegeNames,
                                     DbTreeModel::DbTreeNodeType objectType, QObject *parent) :
    GenericEditableTableModel(headerTitles, parent),
    perspective(perspective),
    privilegeNames(privilegeNames),
    objectType(objectType)
{

}

QVariant ObjectGrantsModel::data ( const QModelIndex & index, int role) const
{
    int column=index.column();

    if(column>GrantSchemaOrObject && column<=privilegeNames.size() && role==Qt::DisplayRole){
        QVariant grantOption=index.data(Qt::EditRole);
        if(grantOption.isValid()){
            return getGrantTypeText(grantOption.toInt());
        }
    }else if(column==GrantSchemaOrObject && role==Qt::DecorationRole){
        bool isDataValid=index.data().isValid();
        if(isDataValid){
            return perspective==OraExp::ObjectGrants ? IconUtil::getIcon("user") : IconUtil::getIcon("table");
        }
    }

    return GenericEditableTableModel::data(index, role);
}


QVariant ObjectGrantsModel::getGrantTypeText(int grantTypeIx) const
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

QString ObjectGrantsModel::isRowDataCorrect(int rowIx) const
{
    QString grantSchema=index(rowIx, ObjectGrantsModel::GrantSchemaOrObject).data().toString().trimmed().toUpper();

    if(grantSchema.isEmpty()){
        return perspective==OraExp::ObjectGrants ? tr("Grantee must be entered") : tr("Object name must be entered");
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

ObjectGrantInfo ObjectGrantsModel::itemInfoFromModelRow(int row) const
{
    ObjectGrantInfo inf;
    inf.objId=-1;

    if(!isRowDataCorrect(row).isEmpty()){
        return inf;
    }

    if(perspective==OraExp::ObjectGrants){
        inf.grantee=index(row, GrantSchemaOrObject).data().toString().trimmed().toUpper();
        inf.objectName=schemaOrObjectName;
    }else{
        inf.grantee=schemaOrObjectName;
        inf.objectName=index(row, GrantSchemaOrObject).data().toString().trimmed().toUpper();
    }

    inf.objectType=this->objectType;

    QString privName;
    for(int i=0; i<privilegeNames.size(); ++i){
        privName=privilegeNames.at(i);
        inf.privileges[privName]=index(row, i+1 /*1st column is schema name*/).data(Qt::EditRole).toInt();
    }

    inf.objId=row+1;

    inf.markedForDeletion=isRowDeleted(row);

    return inf;
}

void ObjectGrantsModel::setSchemaOrObjectName(const QString &schemaOrObjectName)
{
    this->schemaOrObjectName=schemaOrObjectName;
}

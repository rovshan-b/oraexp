#include "objectgrantsmodel.h"
#include "util/iconutil.h"
#include "util/dbutil.h"
#include <QDebug>

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

    if(column>=offset() && column<offset()+privilegeNames.size() && role==Qt::DisplayRole){
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
    QString grantee=index(rowIx, ObjectGrantsModel::GrantSchema).data().toString().trimmed().toUpper();

    if(grantee.isEmpty()){
        return tr("Grantee must be entered");
    }

    if(perspective==OraExp::UserGrants &&
            index(rowIx, ObjectGrantsModel::GrantObject).data().toString().trimmed().toUpper().isEmpty()){
        return tr("Object name must be entered");
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
        inf.grantee=index(row, GrantSchema).data().toString().trimmed().toUpper();
        inf.objectName=schemaOrObjectName;
    }else{
        inf.grantee=schemaOrObjectName;
        inf.objectName=QString("\"%1\".\"%2\"").arg(index(row, GrantSchema).data().toString().trimmed().toUpper(),
                                                    index(row, GrantObject).data().toString().trimmed().toUpper());
    }

    inf.objectType=this->objectType;

    QString privName;
    for(int i=0; i<privilegeNames.size(); ++i){
        privName=privilegeNames.at(i);
        inf.privileges[privName]=index(row, i+offset()).data(Qt::EditRole).toInt();
    }

    inf.objId=row+1;

    inf.markedForDeletion=isRowDeleted(row);

    return inf;
}

void ObjectGrantsModel::setSchemaOrObjectName(const QString &schemaOrObjectName)
{
    this->schemaOrObjectName=schemaOrObjectName;
}

int ObjectGrantsModel::offset() const
{
    return this->perspective==OraExp::ObjectGrants ? 1 : 2;
}

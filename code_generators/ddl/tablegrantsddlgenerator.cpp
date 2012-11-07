#include "tablegrantsddlgenerator.h"
#include "util/strutil.h"

TableGrantsDdlGenerator::TableGrantsDdlGenerator()
{
}

QString TableGrantsDdlGenerator::generateDdl(const TableInfo &tableInfo)
{
    QString ddl;

    //QString fullTableName=QString("\"%1\".\"%2\"").arg(tableInfo.generalInfo.schema, tableInfo.generalInfo.tableName).toUpper();

    int ddlCount=tableInfo.grants.size();
    QList< NameQueryPair > ddlList;
    NameQueryPair ddlItem;
    for(int i=0; i<ddlCount; ++i){
        const ObjectGrantInfo &grantInfo=tableInfo.grants.at(i);
        ddlList=grantInfo.generateDdl();
        for(int k=0; k<ddlList.size(); ++k){
            ddlItem=ddlList.at(k);
            if(!ddlItem.second.isEmpty()){
                addEOL(ddl);
                ddl.append(ddlItem.second).append(";");
            }
        }

    }

    return ddl;
}

QList<NameQueryPair> TableGrantsDdlGenerator::generateAlterDdl(const TableInfo &tableInfo)
{
    QList< NameQueryPair > result;

    //const TableGeneralInfo *generalInfo=&tableInfo.generalInfo;

    //OraExp::TableType tableType=generalInfo->tableType;

    //QString schema=generalInfo->schema;
    //QString tableName=generalInfo->tableName;
    //QString fullTableName=QString("\"%1\".\"%2\"").arg(schema).arg(tableName);

    QList<ObjectGrantInfo> *originalGrantList=&tableInfo.originalInfo()->grants;
    Q_ASSERT(originalGrantList->size()<=tableInfo.grants.size());

    int rowCount=tableInfo.grants.size();

    for(int i=0; i<rowCount; ++i){
        const ObjectGrantInfo &grantInfo=tableInfo.grants.at(i);

        if(i<originalGrantList->size()){
            const ObjectGrantInfo &originalGrantInfo=originalGrantList->at(i);
            QList< NameQueryPair > alterDdls=grantInfo.generateDiffDdl(originalGrantInfo);
            NameQueryPair ddlPair;
            for(int i=0; i<alterDdls.size(); ++i){
                ddlPair=alterDdls.at(i);
                result.append(qMakePair(ddlPair.first, ddlPair.second));
            }
        }else{
            result.append(grantInfo.generateDdl());
        }
    }

    return result;
}

QString TableGrantsDdlGenerator::generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo)
{
    QString ddl;

    QString fullTargetTableName=QString("\"%1\".\"%2\"").arg(targetTableInfo.generalInfo.schema).arg(targetTableInfo.generalInfo.tableName);


    bool found;

    for(int i=0; i<sourceTableInfo.grants.size(); ++i){
        ObjectGrantInfo sourceGrant=sourceTableInfo.grants.at(i); //get a copy
        const ObjectGrantInfo &targetGrant=targetTableInfo.findGrantByGrantee(sourceGrant.grantee, found);

        sourceGrant.objectName=fullTargetTableName; //will generate query for target table
        if(found){ //check if we need to alter it
            QList<NameQueryPair> ddlList=sourceGrant.generateDiffDdl(targetGrant);
            foreach(NameQueryPair pair, ddlList){
                addEOL(ddl);
                ddl.append(pair.second).append(";");
            }
        }else{ //add it
            QList<NameQueryPair> ddlList=sourceGrant.generateDdl();

            foreach(NameQueryPair pair, ddlList){
                addEOL(ddl);
                ddl.append(pair.second).append(";");
            }
        }
    }

    for(int i=0; i<targetTableInfo.grants.size(); ++i){
        const ObjectGrantInfo &targetGrant=targetTableInfo.grants.at(i);
        sourceTableInfo.findGrantByGrantee(targetGrant.grantee, found);
        if(found){
            continue;
        }

        addEOL(ddl);
        ddl.append(targetGrant.generateRevokeAllDdl()).append(";");
    }

    return ddl;
}


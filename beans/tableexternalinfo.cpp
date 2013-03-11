#include "tableexternalinfo.h"

TableExternalInfo::TableExternalInfo() : rejectLimit(-1), rejectLimitUnlimited(false),
    projectColumn(0), parallel(0), parallelDegree(-1)
{
}

TableExternalInfo TableExternalInfo::fromFetchResult(const FetchResult &result)
{
    TableExternalInfo info;

    info.driverType = result.colValue("TYPE_NAME");
    info.defaultDirectory = QString("\"%1\".\"%2\"")
            .arg(result.colValue("DEFAULT_DIRECTORY_OWNER"))
            .arg(result.colValue("DEFAULT_DIRECTORY_NAME"));

    bool parseOk;
    QString strRejectLimit=result.colValue("REJECT_LIMIT");
    if(strRejectLimit=="UNLIMITED"){
        info.rejectLimitUnlimited=true;
    }else{
        int rejectLimit=strRejectLimit.toInt(&parseOk);
        if(parseOk){
            info.rejectLimit=rejectLimit;
        }
    }

    if(result.columnTitles.contains("PROPERTY")){
        QString projectColumnType=result.colValue("PROPERTY");
        if(projectColumnType=="ALL"){
            info.projectColumn=1;
        }else if(projectColumnType=="REFERENCED"){
            info.projectColumn=2;
        }
    }

    QString degree=result.colValue("DEGREE");
    info.parallel=(degree=="1" ? 2 : 1);
    if(degree!="1" && degree!="DEFAULT"){
        info.parallelDegree=degree.toInt();
    }

    info.accessParameters=result.colValue("ACCESS_PARAMETERS").trimmed();

    return info;
}

QString TableExternalInfo::generateDdl() const
{
    QString ddl;

    ddl.append("\n(\nTYPE ").append(driverType).
        append("\n").append("DEFAULT DIRECTORY ").append(defaultDirectory).
        append("\n").append("ACCESS PARAMETERS").
        append("\n(\n").append("%1");

    if(usingClobSubquery){
        ddl.append("\n\tUSING CLOB SUBQUERY");
    }

    ddl.append("\n)");

    ddl.append("\n").append("LOCATION (");


    //add locations
    int locationCount=locations.size();
    QString fileName, directoryName;

    QString locationsDdl;
    for(int i=0; i<locationCount; ++i){
        fileName=locations.at(i).second;
        if(!fileName.isEmpty()){
            if(!locationsDdl.isEmpty()){
                locationsDdl.append(", ");
            }

            directoryName=locations.at(i).first;
            if(!directoryName.isEmpty()){
                locationsDdl.append(directoryName).append(":");
            }

            locationsDdl.append("'").append(fileName).append("'");
        }
    }
    //end add locations
    ddl.append(locationsDdl);
    ddl.append(")\n)");

    if(rejectLimitUnlimited){
        ddl.append("\nREJECT LIMIT UNLIMITED");
    }else if(rejectLimit!=-1){
        ddl.append("\nREJECT LIMIT ").append(QString::number(rejectLimit));
    }

    if(parallel==1){
        ddl.append("\nPARALLEL");
        if(parallelDegree!=-1){
            ddl.append(" ").append(QString::number(parallelDegree));
        }
    }else if(parallel==2){
        ddl.append("\nNOPARALLEL");
    }

    return ddl.arg(accessParameters);
}

QList< NameQueryPair > TableExternalInfo::generateAlterDdl(const QString &fullTableName, const TableExternalInfo &originalInfo) const
{
    QList< NameQueryPair > result;

    result.append(generateDiffDdl(originalInfo, fullTableName));
    if(projectColumn!=originalInfo.projectColumn){
        result.append(qMakePair(QString("alter_external_table_project_column"), generateProjectColumnDdl(fullTableName)));
    }

    return result;
}

QList< NameQueryPair > TableExternalInfo::generateDiffDdl(const TableExternalInfo &other, const QString &fullTableName) const
{
    QList< NameQueryPair > result;

    QString ddl;

    if(defaultDirectory!=other.defaultDirectory){
        ddl.append("\nDEFAULT DIRECTORY ").append(defaultDirectory);
    }

    if(accessParameters!=other.accessParameters){
        ddl.append("\n").append("ACCESS PARAMETERS")
           .append("\n(\n").append(accessParameters)
           .append("\n)");
    }

    if(locations!=other.locations){
        ddl.append("\n").append("LOCATION (");
        QString locationsDdl;
        QString fileName;
        QString directoryName;
        for(int i=0; i<locations.size(); ++i){
            fileName=locations.at(i).second;
            if(!fileName.isEmpty()){
                if(!locationsDdl.isEmpty()){
                    locationsDdl.append(", ");
                }

                directoryName=locations.at(i).first;
                if(!directoryName.isEmpty()){
                    locationsDdl.append(directoryName).append(":");
                }

                locationsDdl.append("'").append(fileName).append("'");
            }
        }
        //end add locations
        ddl.append(locationsDdl);
        ddl.append(")");
    }

    if(rejectLimit!=other.rejectLimit){
        if(rejectLimitUnlimited){
            ddl.append("\nREJECT LIMIT UNLIMITED");
        }else if(rejectLimit!=-1){
            ddl.append("\nREJECT LIMIT ").append(QString::number(rejectLimit));
        }
    }

    if(parallel!=other.parallel || parallelDegree!=other.parallelDegree){
        if(parallel==1){
            ddl.append("\nPARALLEL");
            if(parallelDegree!=-1){
                ddl.append(" ").append(QString::number(parallelDegree));
            }
        }else if(parallel==2){
            ddl.append("\nNOPARALLEL");
        }
    }

    if(!ddl.isEmpty()){
        ddl.prepend(QString("ALTER TABLE %1").arg(fullTableName));
        result.append(qMakePair(QString("alter_external_table_attributes"), ddl));
    }

    return result;
}

QString TableExternalInfo::generateProjectColumnDdl(const QString &fullTableName) const
{
    QString ddl;

    if(projectColumn==1){
        ddl.append("PROJECT COLUMN ALL");
    }else if(projectColumn==2){
        ddl.append("PROJECT COLUMN REFERENCED");
    }

    if(!ddl.isEmpty()){
        ddl.prepend(QString("ALTER TABLE %1 ").arg(fullTableName));
    }

    return ddl;
}

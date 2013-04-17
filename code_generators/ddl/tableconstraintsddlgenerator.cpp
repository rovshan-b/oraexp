#include "tableconstraintsddlgenerator.h"
#include "util/strutil.h"

TableConstraintsDdlGenerator::TableConstraintsDdlGenerator()
{
}

QString TableConstraintsDdlGenerator::generateKeyConstraintsDdl(const TableInfo &tableInfo,
                                                                QList<OraExp::ConstraintType> constraintTypes,
                                                                bool generateAsAlterTable, const QString &fullTableName)
{
    QString ddl;

    int ddlCount=tableInfo.constraints.size();
    QString ddlLine;
    for(int i=0; i<ddlCount; ++i){
        const ConstraintInfo consInfo=tableInfo.constraints.at(i);
        //generate ddl only for required constraint types
        if(!constraintTypes.isEmpty() && !constraintTypes.contains(consInfo.type)){
            continue;
        }

        ddlLine=consInfo.generateDdl(tableInfo.generalInfo.tableType, generateAsAlterTable, fullTableName);

        if(generateAsAlterTable){
            ddlLine.append(";");
        }

        if(!ddl.isEmpty()){
            if(!generateAsAlterTable){
                ddl.append(",");
            }
            ddl.append("\n");
        }

        ddl.append(ddlLine);
    }

    return ddl;
}

QList<NameQueryPair> TableConstraintsDdlGenerator::generateKeyConstraintsAlterDdl(const TableInfo &tableInfo)
{
    QList< NameQueryPair > result;

    const TableGeneralInfo *generalInfo=&tableInfo.generalInfo;

    OraExp::TableType tableType=generalInfo->tableType;

    QString schema=generalInfo->schema;
    QString tableName=generalInfo->tableName;
    QString fullTableName=QString("\"%1\".\"%2\"").arg(schema).arg(tableName);

    QList<ConstraintInfo> *originalConstraintList=&tableInfo.originalInfo()->constraints;
    Q_ASSERT(originalConstraintList->size()<=tableInfo.constraints.size());

    int rowCount=tableInfo.constraints.size();

    for(int i=0; i<rowCount; ++i){
        const ConstraintInfo &consInfo=tableInfo.constraints.at(i);

        if(i<originalConstraintList->size()){
            const ConstraintInfo &originalConsInfo=originalConstraintList->at(i);
            QList< NameQueryPair > alterDdls=consInfo.generateDiffDdl(originalConsInfo, fullTableName, tableType);
            NameQueryPair ddlPair;
            for(int i=0; i<alterDdls.size(); ++i){
                ddlPair=alterDdls.at(i);
                result.append(qMakePair(ddlPair.first, ddlPair.second));
            }
        }else{
            result.append(qMakePair(QString("add_table_constraint_%1").arg(i+1), consInfo.generateDdl(tableType, true, fullTableName)));
        }
    }

    return result;
}

QString TableConstraintsDdlGenerator::generateCheckConstraintsDdl(const TableInfo &tableInfo, bool generateAsAlterTable, const QString &fullTableName)
{
    QString ddl;

    int ddlCount=tableInfo.checkConstraints.size();
    QString ddlLine;
    for(int i=0; i<ddlCount; ++i){
        ddlLine=tableInfo.checkConstraints.at(i).generateDdl(tableInfo.generalInfo.tableType, generateAsAlterTable, fullTableName);

        if(!ddl.isEmpty()){
            if(!generateAsAlterTable){
                ddl.append(",");
            }
            ddl.append("\n");
        }
        ddl.append(ddlLine);
    }

    return ddl;
}

QList<NameQueryPair> TableConstraintsDdlGenerator::generateCheckConstraintsAlterDdl(const TableInfo &tableInfo)
{
    QList< NameQueryPair > result;

    const TableGeneralInfo *generalInfo=&tableInfo.generalInfo;

    OraExp::TableType tableType=generalInfo->tableType;

    QString fullTableName=QString("\"%1\".\"%2\"").arg(generalInfo->schema).arg(generalInfo->tableName);

    const QList<ConstraintInfo> *originalConstraintList=&tableInfo.originalInfo()->checkConstraints;
    Q_ASSERT(originalConstraintList->size()<=tableInfo.checkConstraints.size());

    int rowCount=tableInfo.checkConstraints.size();
    for(int i=0; i<rowCount; ++i){
        const ConstraintInfo &consInfo=tableInfo.checkConstraints.at(i);

        if(i<originalConstraintList->size()){
            const ConstraintInfo &originalConsInfo=originalConstraintList->at(i);
            QList< NameQueryPair > alterDdls=consInfo.generateDiffDdl(originalConsInfo, fullTableName, tableType);
            NameQueryPair ddlPair;
            for(int i=0; i<alterDdls.size(); ++i){
                ddlPair=alterDdls.at(i);
                result.append(qMakePair(ddlPair.first, ddlPair.second));
            }
        }else{
            QString appendColumnDdl;
            appendColumnDdl.append(QString("ALTER TABLE %1 ADD %2").arg(fullTableName, consInfo.generateDdl(tableType).trimmed()));

            result.append(qMakePair(QString("add_table_constraint_%1").arg(i+1), appendColumnDdl));
        }
    }

    return result;
}


QString TableConstraintsDdlGenerator::generateDiffDdl(QList<OraExp::ConstraintType> constraintTypes, const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo, const QStringList &droppedColumnNames)
{
    QString ddl;

    QString fullTargetTableName=QString("\"%1\".\"%2\"").arg(targetTableInfo.generalInfo.schema).arg(targetTableInfo.generalInfo.tableName);

    QList<ConstraintInfo> allConstraints=targetTableInfo.constraints;
    if(constraintTypes.contains(OraExp::CheckConstraint)){
        allConstraints.append(targetTableInfo.checkConstraints);
    }

    bool found;

    for(int i=0; i<allConstraints.size(); ++i){
        const ConstraintInfo &targetCons=allConstraints.at(i);
        if(!constraintTypes.contains(targetCons.type)){
            continue;
        }

        //sourceTableInfo.findConstraintByName(targetCons.type, targetCons.name, found);
        sourceTableInfo.findConstraintByColumns(targetCons.type, targetCons.columns, found);
        if(found){
            continue;
        }

        //check if column for this constraint is dropped. if so this constraint will automatically be dropped
        if(existsInCollection(targetCons.columns, droppedColumnNames)){
            continue;
        }

        addEOL(ddl);
        ddl.append(targetCons.generateDropDdl(fullTargetTableName)).append(";");
    }


    allConstraints=sourceTableInfo.constraints;
    if(constraintTypes.contains(OraExp::CheckConstraint)){
        allConstraints.append(sourceTableInfo.checkConstraints);
    }

    for(int i=0; i<allConstraints.size(); ++i){
        const ConstraintInfo &sourceCons=allConstraints.at(i);
        if(!constraintTypes.contains(sourceCons.type)){
            continue;
        }

        const ConstraintInfo &targetCons=targetTableInfo.findConstraintByColumns(sourceCons.type, sourceCons.columns, found);

        if(found){ //check if we need to alter it
            if(existsInCollection(targetCons.columns, droppedColumnNames)){ //constraint already dropped, so we need to create it as new
                addEOL(ddl);
                ddl.append(sourceCons.generateDdl(targetTableInfo.generalInfo.tableType, true, fullTargetTableName)).append(";");
            }else{
                QList<NameQueryPair> ddlList=sourceCons.generateDiffDdl(targetCons, fullTargetTableName, targetTableInfo.generalInfo.tableType);
                foreach(NameQueryPair pair, ddlList){
                    addEOL(ddl);
                    ddl.append(pair.second).append(";");
                }
            }
        }else{ //add it
            addEOL(ddl);
            ddl.append(sourceCons.generateDdl(targetTableInfo.generalInfo.tableType, true, fullTargetTableName)).append(";");
        }
    }

    return ddl;
}

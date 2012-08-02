#include "tabletriggersddlgenerator.h"
#include "util/strutil.h"

TableTriggersDdlGenerator::TableTriggersDdlGenerator()
{
}

QString TableTriggersDdlGenerator::generateDdl(const TableInfo &tableInfo)
{
    QString ddl;

    QString fullTableName=QString("\"%1\".\"%2\"").arg(tableInfo.generalInfo.schema, tableInfo.generalInfo.tableName).toUpper();

    int ddlCount=tableInfo.triggers.size();
    QString ddlLine;
    for(int i=0; i<ddlCount; ++i){
        const TriggerInfo &triggerInfo=tableInfo.triggers.at(i);
        ddlLine=triggerInfo.generateDdl(fullTableName);
        if(!ddlLine.isEmpty()){
            addEOL(ddl);
            addEOL(ddl);
            ddl.append(ddlLine);
        }
    }

    return ddl;
}

QString TableTriggersDdlGenerator::generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo)
{
    QString ddl;

    QString fullTargetTableName=QString("\"%1\".\"%2\"").arg(targetTableInfo.generalInfo.schema).arg(targetTableInfo.generalInfo.tableName);


    bool found;
    QString ddlLine;
    for(int i=0; i<sourceTableInfo.triggers.size(); ++i){
        const TriggerInfo &sourceTrg=sourceTableInfo.triggers.at(i);
        const TriggerInfo &targetTrg=targetTableInfo.findTriggerByName(sourceTrg.name, found);

        ddlLine=found ? sourceTrg.generateDiffDdl(targetTrg, fullTargetTableName) : sourceTrg.generateDdl(fullTargetTableName);
        if(!ddlLine.isEmpty()){
            addEOL(ddl);
            ddl.append(ddlLine);
        }
    }

    for(int i=0; i<targetTableInfo.triggers.size(); ++i){
        const TriggerInfo &targetTrg=targetTableInfo.triggers.at(i);
        sourceTableInfo.findTriggerByName(targetTrg.name, found);
        if(found){
            continue;
        }

        addEOL(ddl);
        ddl.append(targetTrg.generateDropDdl()).append(";");
    }

    return ddl;
}

#include "tableddlgenerator.h"
#include "beans/tablegeneralinfo.h"
#include "tablecolumnsddlgenerator.h"
#include "tableconstraintsddlgenerator.h"
#include "tableindexesddlgenerator.h"
#include "tablepartitionsddlgenerator.h"
#include "tabletriggersddlgenerator.h"
#include "tablegrantsddlgenerator.h"
#include "util/strutil.h"

TableDdlGenerator::TableDdlGenerator()
{
}

QString TableDdlGenerator::generateDdl(const TableInfo &tableInfo,
                                       bool includeInlinePkConstraint,
                                       bool includeConstraints,
                                       QList<OraExp::ConstraintType> constraintTypes,
                                       bool includeGrants, const TableCreateDdlOptions &options)
{
    const TableGeneralInfo *generalInfo=&tableInfo.generalInfo;

    QString createTableStatement = generalInfo->generateDdl(!tableInfo.partitioningInfo.isEmpty,
                                                            options.newObjectOptions.storage, options.newObjectOptions.tablespaceOnly,
                                                            options.iotProperties, options.properties,
                                                            options.flashbackArchive);

    if(createTableStatement.isEmpty()){
        return "--Definition is not complete. Table name and column list must be entered.";
    }

    QString columnsDdl=TableColumnsDdlGenerator::generateDdl(tableInfo, includeInlinePkConstraint);

    if(columnsDdl.isEmpty()){
        return "--Definition is not complete. Table name and column list must be entered.";
    }

    OraExp::TableType tableType=generalInfo->tableType;

    QString constraintsDdl;
    if(includeConstraints){
        constraintsDdl=generateConstraintsDdl(tableInfo, constraintTypes, false);
    }

    QString lobParamsDdl;

    if(tableType!=OraExp::TableTypeTemporaryTransaction &&
            tableType!=OraExp::TableTypeTemporarySession &&
            tableType!=OraExp::TableTypeExternal &&
            options.lobProperties){
        lobParamsDdl=TableColumnsDdlGenerator::generateLobParamsDdl(tableInfo);
    }

    createTableStatement=createTableStatement
            .arg(columnsDdl)
            .arg(constraintsDdl)
            .arg(lobParamsDdl.isEmpty() ? "" : " "+lobParamsDdl)
            .arg(tableInfo.partitioningInfo.generateDdl(generalInfo->tableType))
            .arg(tableType==OraExp::TableTypeExternal ? tableInfo.externalInfo.generateDdl() : "");

    if(tableType==OraExp::TableTypeExternal){
        QString fullTableName=QString("\"%1\".\"%2\"").arg(generalInfo->schema).arg(generalInfo->tableName);
        QString projectColumnDdl=tableInfo.externalInfo.generateProjectColumnDdl(fullTableName);
        if(!projectColumnDdl.isEmpty()){
            createTableStatement.append("\n\n").append(projectColumnDdl).append(";");
        }
    }


    QString commentsDdl=TableColumnsDdlGenerator::generateCommentsDdl(tableInfo);
    if(!commentsDdl.isEmpty()){
        createTableStatement.append("\n").append(commentsDdl);
    }

    if(tableType!=OraExp::TableTypeExternal && options.indexes){
        QString indexesDdl=TableIndexesDdlGenerator::generateDdl(tableInfo, options.newObjectOptions.storage, options.newObjectOptions.tablespaceOnly);
        if(!indexesDdl.isEmpty()){
            createTableStatement.append("\n").append(indexesDdl);
        }
    }

    if(includeGrants && options.grants){
        QString grantsDdl=TableGrantsDdlGenerator::generateDdl(tableInfo);
        if(!grantsDdl.isEmpty()){
            createTableStatement.append("\n").append(grantsDdl);
        }
    }

    return createTableStatement;
}

QString TableDdlGenerator::generateConstraintsDdl(const TableInfo &tableInfo,
                                                  QList<OraExp::ConstraintType> constraintTypes,
                                                  bool generateAsAlterTable)
{
    QString constraintsDdl;

    QString fullTableName;
    if(generateAsAlterTable){
        fullTableName=QString("\"%1\".\"%2\"").arg(tableInfo.generalInfo.schema).arg(tableInfo.generalInfo.tableName);
    }

    if(tableInfo.generalInfo.tableType!=OraExp::TableTypeExternal){
        //this function will check for required constraint types itself
        QString keyConstraintsDdl=TableConstraintsDdlGenerator::generateKeyConstraintsDdl(tableInfo,
                                                                                          constraintTypes,
                                                                                          generateAsAlterTable,
                                                                                          fullTableName);
        if(!keyConstraintsDdl.isEmpty()){
            if(!generateAsAlterTable){
                constraintsDdl.append(",\n");
            }else{
                addEOL(constraintsDdl);
            }
            constraintsDdl.append(keyConstraintsDdl);
        }

        //check if we need to generate ddl for check constraints
        if(constraintTypes.isEmpty() || constraintTypes.contains(OraExp::CheckConstraint)){
            QString checkConstraintsDdl=TableConstraintsDdlGenerator::generateCheckConstraintsDdl(tableInfo, generateAsAlterTable, fullTableName);
            if(!checkConstraintsDdl.isEmpty()){
                if(!generateAsAlterTable){
                    constraintsDdl.append(",\n");
                }else{
                    addEOL(constraintsDdl);
                }
                constraintsDdl.append(checkConstraintsDdl);
            }
        }
    }

    return constraintsDdl;
}

QPair<QString, QString> TableDdlGenerator::generateDdlForExporting(const TableInfo &tableInfo, const TableCreateDdlOptions &options)
{
    QList<OraExp::ConstraintType> constraintTypes;
    constraintTypes.append(OraExp::PrimaryKey);
    constraintTypes.append(OraExp::UniqueKey);
    constraintTypes.append(OraExp::CheckConstraint);
    QString tableDdl = tableInfo.generateDdl(false, true, constraintTypes, false, options);

    QString startDdl;
    startDdl.append(QString("\n--DDL for creating table %1\n").arg(tableInfo.generalInfo.tableName));
    startDdl.append(tableDdl);
    startDdl.append(QString("\n--end DDL for creating table %1").arg(tableInfo.generalInfo.tableName));


    constraintTypes.clear();
    constraintTypes.append(OraExp::ForeignKey);
    QString constraintsDdl = TableDdlGenerator::generateConstraintsDdl(tableInfo, constraintTypes, true);
    QString triggersDdl=options.triggers ? TableTriggersDdlGenerator::generateDdl(tableInfo) : "";
    QString grantsDdl = options.grants ? TableGrantsDdlGenerator::generateDdl(tableInfo) : "";

    QString endDdl;
    if(!constraintsDdl.isEmpty()){
        endDdl.append(QString("\n--Foreign key constraints for table %1\n").arg(tableInfo.generalInfo.tableName));
        endDdl.append(constraintsDdl);
        endDdl.append(QString("\n--end foreign key constraints for table %1").arg(tableInfo.generalInfo.tableName));
    }
    if(!triggersDdl.isEmpty()){
        endDdl.append(QString("\n--Triggers for table %1\n").arg(tableInfo.generalInfo.tableName));
        endDdl.append(triggersDdl);
        endDdl.append(QString("\n--end triggers for table %1").arg(tableInfo.generalInfo.tableName));
    }
    if(!grantsDdl.isEmpty()){
        endDdl.append(QString("\n--Grants for table %1\n").arg(tableInfo.generalInfo.tableName));
        endDdl.append(grantsDdl);
        endDdl.append(QString("\n--end grants for table %1").arg(tableInfo.generalInfo.tableName));
    }

    return qMakePair(startDdl, endDdl);
}

QPair<QString, QString> TableDdlGenerator::generateDiffDdl(const TableInfo &sourceTableInfo, const TableInfo &targetTableInfo, const TableDiffDdlOptions &options)
{
    QString startDdl;
    QString endDdl;

    QList<NameQueryPair> queryList;

    //generate diff ddl for general info
    if(sourceTableInfo.generalInfo.tableType!=OraExp::TableTypeExternal){
        queryList = sourceTableInfo.generalInfo.generateAlterDdl(targetTableInfo.generalInfo,
                                                                 options.comments,
                                                                 options.properties,
                                                                 options.flashbackArchive,
                                                                 options.storage,
                                                                 options.iotProperties);
        QString generalInfoDiffDdl;
        foreach(NameQueryPair pair, queryList){
            addEOL(generalInfoDiffDdl);
            generalInfoDiffDdl.append(pair.second).append(";");
        }
        if(!generalInfoDiffDdl.isEmpty()){
            startDdl.append("\n").append(generalInfoDiffDdl);
        }
    }

    QStringList droppedColumnNames;

    //generate diff ddl for columns
    QString columnsDiffDdl=options.columns ? TableColumnsDdlGenerator::generateDiffDdl(sourceTableInfo, targetTableInfo,
                                                                                       droppedColumnNames, options.comments, options.lobProperties,
                                                                                       options.newObjectOptions.storage, options.newObjectOptions.tablespaceOnly) : "";

    if(!columnsDiffDdl.isEmpty()){
        addEOL(startDdl);
        startDdl.append(columnsDiffDdl);
    }

    //generate diff ddl for constraints
    QList<OraExp::ConstraintType> constraintTypes;
    constraintTypes.append(OraExp::PrimaryKey);
    constraintTypes.append(OraExp::UniqueKey);
    constraintTypes.append(OraExp::CheckConstraint);
    QString constraintsDiffDdl=options.constraints ? TableConstraintsDdlGenerator::generateDiffDdl(constraintTypes, sourceTableInfo,
                                                                                                   targetTableInfo, droppedColumnNames) : "";

    if(!constraintsDiffDdl.isEmpty()){
        addEOL(startDdl);
        startDdl.append(constraintsDiffDdl);
    }

    //generate diff ddl for fk constraints
    constraintTypes.clear();
    constraintTypes.append(OraExp::ForeignKey);
    QString fkConstraintsDiffDdl=options.constraints ? TableConstraintsDdlGenerator::generateDiffDdl(constraintTypes, sourceTableInfo,
                                                                                                     targetTableInfo, droppedColumnNames) : "";

    if(!fkConstraintsDiffDdl.isEmpty()){
        addEOL(endDdl);
        endDdl.append(QString("\n--Foreign key constraints for table %1\n").arg(targetTableInfo.generalInfo.tableName));
        endDdl.append(fkConstraintsDiffDdl);
        endDdl.append(QString("\n--end foreign key constraints for table %1").arg(targetTableInfo.generalInfo.tableName));
    }

    //generate diff ddl for indexes
    QString indexesDiffDdl=options.indexes ? TableIndexesDdlGenerator::generateDiffDdl(sourceTableInfo, targetTableInfo, droppedColumnNames,
                                                                                       options.newObjectOptions.storage, options.newObjectOptions.tablespaceOnly) : "";

    if(!indexesDiffDdl.isEmpty()){
        addEOL(startDdl);
        startDdl.append(indexesDiffDdl);
    }

    //generate diff ddl for partitions
    QString partitionsDiffDdl=options.partitions ? TablePartitionsDdlGenerator::generateDiffDdl(sourceTableInfo, targetTableInfo, false) : "";
    if(!partitionsDiffDdl.isEmpty()){
        addEOL(startDdl);
        startDdl.append(partitionsDiffDdl);
    }

    //generate diff ddl for external table info
    if(options.externalProperties && sourceTableInfo.generalInfo.tableType==OraExp::TableTypeExternal && targetTableInfo.generalInfo.tableType==OraExp::TableTypeExternal){
        queryList=sourceTableInfo.externalInfo.generateAlterDdl(targetTableInfo.generalInfo.getFullTableName(), targetTableInfo.externalInfo);
        foreach(NameQueryPair pair, queryList){
            addEOL(startDdl);
            startDdl.append(pair.second).append(";");
        }
    }

    //generate diff for triggers
    QString triggersDiffDdl=options.triggers ? TableTriggersDdlGenerator::generateDiffDdl(sourceTableInfo, targetTableInfo) : "";
    if(!triggersDiffDdl.isEmpty()){
        addEOL(endDdl);
        endDdl.append(QString("--Triggers for table %1\n").arg(targetTableInfo.generalInfo.tableName));
        endDdl.append(triggersDiffDdl);
        endDdl.append(QString("\n--end triggers for table %1").arg(targetTableInfo.generalInfo.tableName));
    }

    //generate diff for grants
    QString grantsDiffDdl=options.grants ? TableGrantsDdlGenerator::generateDiffDdl(sourceTableInfo, targetTableInfo) : "";
    if(!grantsDiffDdl.isEmpty()){
        addEOL(startDdl);
        startDdl.append(grantsDiffDdl);
    }

    if(!startDdl.isEmpty()){
        startDdl.prepend(QString("\n--DDL for altering table %1\n").arg(targetTableInfo.generalInfo.tableName));
        startDdl.append(QString("\n--end DDL for altering table %1").arg(targetTableInfo.generalInfo.tableName));
    }

    return qMakePair(startDdl, endDdl);
}

QList< QueryListItem > TableDdlGenerator::generateAlterDdl(const TableInfo &tableInfo, const QHash<QString,QObject*> &requesters)
{
    const TableGeneralInfo *generalInfo=&tableInfo.generalInfo;
    const TableInfo *originalTableInfo=tableInfo.originalInfo();

    Q_ASSERT(originalTableInfo);

    QList< QueryListItem > result;

    result.append(QueryListItem(requesters.value("general_info") , tableInfo.generalInfo.generateAlterDdl(originalTableInfo->generalInfo)));
    result.append(QueryListItem(requesters.value("columns"), TableColumnsDdlGenerator::generateAlterDdl(tableInfo)));
    result.append(QueryListItem(requesters.value("constraints"), TableConstraintsDdlGenerator::generateKeyConstraintsAlterDdl(tableInfo)));
    result.append(QueryListItem(requesters.value("check_constraints"), TableConstraintsDdlGenerator::generateCheckConstraintsAlterDdl(tableInfo)));
    result.append(QueryListItem(requesters.value("indexes"), TableIndexesDdlGenerator::generateAlterDdl(tableInfo)));

    if(!originalTableInfo->partitioningInfo.isEmpty && !tableInfo.partitioningInfo.isEmpty){
        result.append(QueryListItem(requesters.value("partitions"), tableInfo.partitioningInfo.generateAlterDdl(originalTableInfo->partitioningInfo,
                                                                                                      generalInfo->schema,
                                                                                                      generalInfo->tableName,
                                                                                                      false)));
    }

    if(originalTableInfo->generalInfo.tableType==OraExp::TableTypeExternal){
        QString fullTableName=QString("\"%1\".\"%2\"").arg(generalInfo->schema).arg(generalInfo->tableName);
        result.append(QueryListItem(requesters.value("external_properties"), tableInfo.externalInfo.generateAlterDdl(fullTableName, originalTableInfo->externalInfo)));
    }

    result.append(QueryListItem(requesters.value("grants"), TableGrantsDdlGenerator::generateAlterDdl(tableInfo)));

    return result;
}

QString TableDdlGenerator::generateDropDdl(const TableInfo &tableInfo)
{
    return QString("DROP TABLE %1 CASCADE CONSTRAINTS").arg(tableInfo.generalInfo.getFullTableName());
}

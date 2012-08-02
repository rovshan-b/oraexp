#include "tablegeneralinfo.h"

TableGeneralInfo::TableGeneralInfo()
{
}

QString TableGeneralInfo::getFullTableName() const
{
    return QString("\"%1\".\"%2\"").arg(schema, tableName);
}

QString TableGeneralInfo::generateDdl(bool isPartitioned, bool storage, bool tablespaceOnly, bool iotProperties, bool attributes, bool flashbackArchive) const
{
    if(schema.isEmpty() || tableName.isEmpty()){
        return "";
    }

    bool isTemporaryTable=(tableType==OraExp::TableTypeTemporaryTransaction || tableType==OraExp::TableTypeTemporarySession);
    bool isExternablTable=(tableType==OraExp::TableTypeExternal);

    QString ddl("CREATE");
    if(isTemporaryTable){
        ddl.append(" GLOBAL TEMPORARY");
    }
    ddl.append(" TABLE \"");
    ddl.append(schema).
        append("\".\"").
        append(tableName).
        append("\"\n").
        append("(\n%1%2\n)"); //placeholders for columns and constraints

    if(isTemporaryTable){
        if(tableType==OraExp::TableTypeTemporaryTransaction){
            ddl.append("\nON COMMIT DELETE ROWS");
        }else{
            ddl.append("\nON COMMIT PRESERVE ROWS");
        }
    }else if(tableType==OraExp::TableTypeExternal){
        ddl.append("\nORGANIZATION EXTERNAL");
    }else if(tableType==OraExp::TableTypeIndexOrganized){
        ddl.append("\nORGANIZATION INDEX");
    }

    if(!isTemporaryTable && !isExternablTable){

        QString attributesDdl=attributes ? additionalAttributes.generateDdl(tableType, isPartitioned, flashbackArchive) : "";
        if(!attributesDdl.isEmpty()){
            ddl.append("\n").append(attributesDdl);
        }

        QString storageDdl=storage ? storageParams.generateDdl(false, tablespaceOnly) : "";
        if(!storageDdl.isEmpty()){
            ddl.append("\n").append(storageDdl);
        }
    }

    if(tableType==OraExp::TableTypeIndexOrganized && iotProperties){
        QString indexOrganizedPropertiesDdl=indexOrganizedProperties.generateDdl();
        if(!indexOrganizedPropertiesDdl.isEmpty()){
            ddl.append("\n").append(indexOrganizedPropertiesDdl);
        }
    }

    ddl.append("%3"); //placeholder is for lob properties

    ddl.append("%4"); //placeholder for partitioning properties

    ddl.append("%5"); //placeholder for external table properties

    ddl.append(";");

    if(!isExternablTable){
        if(!comments.isEmpty()){
            QString commentsToEscape=comments;
            ddl.append("\n\nCOMMENT ON TABLE \"").append(schema).append("\".\"").append(tableName).append("\" ").
                    append("IS '").append(commentsToEscape.replace("'", "''")).append("';");
        }
    }

    return ddl;
}

QList< NameQueryPair > TableGeneralInfo::generateAlterDdl(const TableGeneralInfo &originalInfo,
                                                          bool tblComments,
                                                          bool attributes,
                                                          bool flashbackArchive,
                                                          bool storage,
                                                          bool iotProperties) const
{
    QList< NameQueryPair > result;

    QString fullTableName=QString("\"%1\".\"%2\"").arg(schema).arg(tableName);

    QString ddl;
    if(tableName!=originalInfo.tableName){
        ddl=QString().append("ALTER TABLE \"")
                .append(schema)
                .append("\".\"")
                .append(originalInfo.tableName)
                .append("\" RENAME TO \"")
                .append(tableName)
                .append("\"");

        result.append(qMakePair<QString,QString>("rename_table", ddl));
    }

    if(tblComments && comments!=originalInfo.comments){
        QString commentsToEscape=comments;
        ddl=QString().append("COMMENT ON TABLE \"").append(schema).append("\".\"").append(tableName).append("\" ").
                append("IS '").append(commentsToEscape.replace("'", "''")).append("'");
        result.append(qMakePair<QString,QString>("alter_table_comments", ddl));
    }

    QString attributesDdl=attributes ? additionalAttributes.generateDiffDdl(originalInfo.additionalAttributes, flashbackArchive) : "";
    if(!attributesDdl.isEmpty()){
        ddl=QString().append("ALTER TABLE ").append(fullTableName).append(attributesDdl);
        result.append(qMakePair<QString,QString>("alter_table_additional_attributes", ddl));
    }

    QString storageDdl=storage ? storageParams.generateDiffDdl(originalInfo.storageParams) : "";
    if(!storageDdl.isEmpty()){
        ddl=QString().append("ALTER TABLE ").append(fullTableName).append(storageDdl);
        result.append(qMakePair<QString,QString>("alter_table_storage", ddl));
    }

    QString indexOrganizedPropertiesDdl=iotProperties ? indexOrganizedProperties.generateDiffDdl(originalInfo.indexOrganizedProperties) : "";
    if(!indexOrganizedPropertiesDdl.isEmpty()){
        ddl=QString().append("ALTER TABLE ").append(fullTableName).append(indexOrganizedPropertiesDdl);
        result.append(qMakePair<QString,QString>("alter_iot_attributes", ddl));
    }

    return result;
}

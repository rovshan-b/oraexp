#include "columninfo.h"
#include "util/dbutil.h"
#include "code_generators/ddl/tablecolumnsddlgenerator.h"

ColumnInfo::ColumnInfo() : markedForDeletion(false), dropped(false)
{
}

bool ColumnInfo::operator==(const ColumnInfo &other) const
{
    return name==other.name &&
           dataType==other.dataType &&
           isNullable==other.isNullable &&
           defaultValue==other.defaultValue &&
           virtualDefinition==other.virtualDefinition &&
           (lobParams.isEmpty || lobParams==other.lobParams) &&
           comments==other.comments;
}

ColumnInfo ColumnInfo::fromFetchResult(const FetchResult &result)
{
    ColumnInfo i;

    i.columnId=result.colValue("COLUMN_ID", 0);
    i.name=result.colValue("COLUMN_NAME");
    i.columnType=result.colValue("VIRTUAL")=="YES" ? OraExp::ColumnTypeVirtual : OraExp::ColumnTypeNormal;
    i.dataType=result.colValue("DATA_TYPE");
    i.isNullable=result.colValue("NULLABLE").startsWith("Y");
    if(i.columnType==OraExp::ColumnTypeVirtual){
        i.virtualDefinition=result.colValue("DATA_DEFAULT").trimmed();
    }else{
        i.defaultValue=result.colValue("DATA_DEFAULT").trimmed();
    }
    i.comments=result.colValue("COMMENTS").trimmed();
    i.isPrimaryKey=result.colValue("IS_PK", 0)==1;
    i.isUnique=result.colValue("IS_UNIQUE", 0)==1;

    //lobParams is not populated here

    return i;
}

QString ColumnInfo::generateDdl(OraExp::TableType tableType, bool includeInlinePkUqConstraints) const
{
    QString ddl;

    ddl.append("\"").append(name.toUpper()).append("\"");

    if(!dataType.isEmpty()){
        ddl.append(" ").append(dataType);
    }

    if(tableType==OraExp::TableTypeHeap && columnType==OraExp::ColumnTypeVirtual){
        ddl.append(" AS ( ").append(virtualDefinition).append(" )");
    }

    if(tableType!=OraExp::TableTypeExternal){

        if(!defaultValue.isEmpty() && columnType!=OraExp::ColumnTypeVirtual){
            ddl.append(" DEFAULT ").append(defaultValue);
        }

        if(!isNullable && !isPrimaryKey){
            ddl.append(" NOT NULL");
        }

        if(includeInlinePkUqConstraints){
            if(isPrimaryKey){
                ddl.append(" PRIMARY KEY");
            }else if(isUnique){
                ddl.append(" UNIQUE");
            }
        }
    }

    return ddl;
}

QString ColumnInfo::generateDropDdl(const QString &fullTableName) const
{
    return QString("ALTER TABLE %1 DROP COLUMN \"%2\"").arg(fullTableName, name);
}

QString ColumnInfo::generateRenameDdl(const QString &fullTableName, const QString &newName) const
{
    return QString("ALTER TABLE %1 RENAME COLUMN \"%2\" TO \"%3\"").arg(fullTableName, name, newName);
}

QList< NameQueryPair > ColumnInfo::generateDiffDdl(const ColumnInfo &other,
                                                   const QString &fullTableName,
                                                   OraExp::TableType /*tableType*/, bool compareComments, bool compareLobParams) const
{
    QList< NameQueryPair > result;
    if(other.dropped){
        return result;
    }

    //QString fullTableName=QString("\"%1\".\"%2\"").arg(schemaName).arg(tableName);

    QString ddl;

    if(markedForDeletion && !other.dropped){
        ddl=other.generateDropDdl(fullTableName);
        result.append(qMakePair(QString("drop_table_column_%1").arg(columnId), ddl));

        return result;
    }

    if(name!=other.name){
        ddl=other.generateRenameDdl(fullTableName, name);
        result.append(qMakePair(QString("rename_table_column_%1").arg(columnId), ddl));
    }

    if(dataType!=other.dataType){
        ddl=QString("ALTER TABLE %1 MODIFY \"%2\" %3").arg(fullTableName, name, dataType);
        result.append(qMakePair(QString("alter_column_data_type_%1").arg(columnId), ddl));
    }

    if(isNullable!=other.isNullable){
        ddl=QString("ALTER TABLE %1 MODIFY \"%2\" %3").arg(fullTableName, name, isNullable ? "NULL" : "NOT NULL");
        result.append(qMakePair(QString("alter_column_nullable_%1").arg(columnId), ddl));
    }

    if(defaultValue!=other.defaultValue){
        ddl=QString("ALTER TABLE %1 MODIFY \"%2\" DEFAULT %3").arg(fullTableName, name, defaultValue);
        result.append(qMakePair(QString("alter_column_default_value_%1").arg(columnId), ddl));
    }

    if(virtualDefinition!=other.virtualDefinition){
        ddl=QString("ALTER TABLE %1 MODIFY \"%2\" AS %3").arg(fullTableName, name, virtualDefinition);
        result.append(qMakePair(QString("alter_column_virtual_definition_%1").arg(columnId), ddl));
    }

    if(compareLobParams && !lobParams.isEmpty){
        QString lobDdl=lobParams.generateDiffDdl(other.lobParams);
        if(!lobDdl.isEmpty()){
            ddl=QString("ALTER TABLE %1 MODIFY LOB (\"%2\") %3").arg(fullTableName, name, lobDdl);
            result.append(qMakePair(QString("alter_column_lob_storage_%1").arg(columnId), ddl));
        }
    }

    if(compareComments && comments!=other.comments){
        ddl=QString("COMMENT ON COLUMN %1.\"%2\" IS '%3'").arg(fullTableName, name, QString(comments).replace("'", "''"));
        result.append(qMakePair(QString("alter_column_comments_%1").arg(columnId), ddl));
    }

    return result;
}

#include "constraintinfo.h"
#include "util/dbutil.h"

ConstraintInfo::ConstraintInfo() : constraintId(-1),
                                   deleteRule(0),
                                   generated(false),
                                   markedForDeletion(false),
                                   dropped(false)
{
}

bool ConstraintInfo::operator==(const ConstraintInfo &other) const
{
    return name==other.name &&
            !needsRecreation(other);
}

ConstraintInfo ConstraintInfo::fromFetchResult(const FetchResult &result)
{
    ConstraintInfo i;

    i.name=result.colValue("CONSTRAINT_NAME");
    i.type=(OraExp::ConstraintType)result.colValue("CONS_TYPE", 4);
    i.columns=result.colValue("COLUMNS");
    i.searchCondition=result.colValue("SEARCH_CONDITION").trimmed();
    i.rOwner=result.colValue("R_OWNER");
    i.rTableName=result.colValue("R_TABLE_NAME");
    i.rConsName=result.colValue("R_CONSTRAINT_NAME");

    QString strDeleteRule=result.colValue("DELETE_RULE");
    if(strDeleteRule=="CASCADE"){
        i.deleteRule=1;
    }else if(strDeleteRule=="SET NULL"){
        i.deleteRule=2;
    }

    i.deferrable=result.colValue("DEFERRABLE")=="DEFERRABLE";
    i.deferred=result.colValue("DEFERRED")=="DEFERRED";
    i.generated=result.colValue("GENERATED")=="GENERATED NAME";

    return i;
}

QString ConstraintInfo::generateDdl(OraExp::TableType tableType, bool generateAsAlterTable, const QString &fullTableName) const
{
    bool isTemporaryTable=(tableType==OraExp::TableTypeTemporaryTransaction || tableType==OraExp::TableTypeTemporarySession);

    if(type==OraExp::ForeignKey && isTemporaryTable){
        return "";
    }

    QString ddl;

    if(generateAsAlterTable){
        ddl.append(QString("ALTER TABLE %1 ADD ").arg(fullTableName));
    }else{
        ddl.append("\t");
    }

    if(!generated){
        ddl.append("CONSTRAINT \"").append(name.toUpper()).append("\" ");
    }

    ddl.append(DbUtil::getConstraintTypeName(type, false).toUpper()).
        append(" (").append(type==OraExp::CheckConstraint ? searchCondition.trimmed() : columns.toUpper()).append(")");

    if(type==OraExp::ForeignKey){
        ddl.append(" REFERENCES ").append("\"").
                append(rOwner).append("\".\"").append(rTableName).
                append("\"").append(" (").append(rColumns).append(")");

        if(deleteRule==1){
            ddl.append(" ON DELETE CASCADE");
        }else if(deleteRule==2){
            ddl.append(" ON DELETE SET NULL");
        }
    }

    if(deferred){
        ddl.append(" INITIALLY DEFERRED");
    }

    if(deferrable){
        ddl.append(" DEFERRABLE");
    }

    return ddl;
}

QString ConstraintInfo::generateDropDdl(const QString &fullTableName) const
{
    return QString("ALTER TABLE %1 DROP CONSTRAINT \"%2\"").arg(fullTableName, name);
}

QList< NameQueryPair > ConstraintInfo::generateDiffDdl(const ConstraintInfo &other,
                                                                const QString &fullTableName,
                                                                OraExp::TableType tableType) const
{
    QList< NameQueryPair > result;

    QString ddl;

    if(markedForDeletion && !other.dropped){
        ddl=other.generateDropDdl(fullTableName);
        result.append(qMakePair(QString("drop_table_constraint_%1").arg(constraintId), ddl));

        return result;
    }else if(needsRecreation(other)){
        ddl=other.generateDropDdl(fullTableName);
        result.append(qMakePair(QString("drop_table_constraint_for_recreation_%1").arg(constraintId), ddl));

        ddl=generateDdl(tableType, true, fullTableName);
        result.append(qMakePair(QString("create_table_constraint_after_dropping_%1").arg(constraintId), ddl));
    }else if(name!=other.name && !generated){
        ddl=QString("ALTER TABLE %1 RENAME CONSTRAINT \"%2\" TO \"%3\"").arg(fullTableName, other.name, name);
        result.append(qMakePair(QString("rename_table_constraint_%1").arg(constraintId), ddl));
    }

    return result;
}

bool ConstraintInfo::needsRecreation(const ConstraintInfo &other) const
{
    bool result;
    if(other.type==OraExp::CheckConstraint){
        result = (type!=other.type ||
            searchCondition!=other.searchCondition ||
            deferrable!=other.deferrable ||
            deferred!=other.deferred);
    }else{
        result = (type!=other.type ||
            columns!=other.columns ||
            rOwner!=other.rOwner ||
            rTableName!=other.rTableName ||
            rColumns!=other.rColumns ||
            deleteRule!=other.deleteRule ||
            deferrable!=other.deferrable ||
            deferred!=other.deferred);
    }

    return result;
}

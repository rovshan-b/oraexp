#include "tabledmlgenerator.h"
#include "util/strutil.h"
#include "util/dbutil.h"
#include "defines.h"
#include <QStringList>

TableDmlGenerator::TableDmlGenerator()
{
}

QString TableDmlGenerator::generateSelectStatement(const QString &schemaName, const QString &tableName)
{
    return QString("SELECT * FROM \"%1\".\"%2\"").arg(schemaName, tableName);
}

QString TableDmlGenerator::generateSelectStatement(const QString &schemaName, const QString &tableName,
                                                   const QStringList &columns, const QStringList &whereColumns,
                                                   OraExp::BindVarStyle bindStyle, bool includeSchema)
{
    QString dml;

    dml.append("SELECT ").append(columns.join(", ")).append(" FROM ");
    if(includeSchema){
        dml.append(schemaName).append(".");
    }
    dml.append(tableName);
    dml.append(TableDmlGenerator::generateWhereClause(columns, whereColumns, bindStyle));

    return dml;
}

QString TableDmlGenerator::generateDeleteStatement(const QString &schemaName, const QString &tableName)
{
    return QString("DELETE FROM \"%1\".\"%2\"").arg(schemaName, tableName);
}

QString TableDmlGenerator::generateDeleteStatement(const QString &schemaName, const QString &tableName,
                                                   const QStringList &columns, const QStringList &whereColumns,
                                                   OraExp::BindVarStyle bindStyle, bool includeSchema)
{
    QString dml;

    dml.append("DELETE FROM ");
    if(includeSchema){
        dml.append(schemaName).append(".");
    }
    dml.append(tableName);
    dml.append(TableDmlGenerator::generateWhereClause(columns, whereColumns, bindStyle));

    return dml;
}

QString TableDmlGenerator::generateUpdateStatement(const QString &schemaName, const QString &tableName,
                                                   const QStringList &columns, const QStringList &whereColumns,
                                                   OraExp::BindVarStyle bindStyle, bool includeSchema)
{
    QString dml;

    dml.append("UPDATE ");
    if(includeSchema){
        dml.append(schemaName).append(".");
    }
    dml.append(tableName).append(" SET ");
    for(int i=0; i<columns.size(); ++i){
        if(i > 0){
            dml.append(", ");
        }
        QString colName = columns.at(i);
        dml.append(colName).append(" = ").append(formatBindVarName(colName, i+1, bindStyle));
    }
    dml.append(TableDmlGenerator::generateWhereClause(columns, whereColumns, bindStyle, columns.size()));

    return dml;
}

QString TableDmlGenerator::generateInsertStatement(const QString &schemaName, const QString &tableName,
                                                   const QStringList &columns,
                                                   OraExp::BindVarStyle bindStyle, bool includeSchema)
{
    QString dml;

    dml.append("INSERT INTO ");
    if(includeSchema){
        dml.append(schemaName).append(".");
    }
    dml.append(tableName);
    dml.append(" (").append(columns.join(", ")).append(")");
    dml.append("\nVALUES (");
    for(int i=0; i<columns.size(); ++i){
        if(i > 0){
            dml.append(", ");
        }
        dml.append(formatBindVarName(columns.at(i), i+1, bindStyle));
    }
    dml.append(")");

    return dml;
}

QString TableDmlGenerator::generateWhereClause(const QStringList &columns, const QStringList &whereColumns, OraExp::BindVarStyle bindStyle, int bindStartPos)
{
    QString result;

    if(!whereColumns.isEmpty()){
        result.append("\nWHERE ").append(TableDmlGenerator::generateWhereBindVars(columns, whereColumns, bindStyle, bindStartPos));
    }

    return result;
}

QString TableDmlGenerator::generateWhereBindVars(const QStringList &columns, const QStringList &whereColumns, OraExp::BindVarStyle bindStyle, int bindStartPos)
{
    QString result;

    for(int i=0; i<whereColumns.size(); ++i){
        if(i > 0){
            result.append(" AND ");
        }
        QString whereColName = whereColumns.at(i);
        result.append(whereColName).append(" = ");

        if(bindStyle == OraExp::BindVarStyleNamed){
            if(columns.contains(whereColName)){
                whereColName = addNumericSuffix(whereColName, columns);

                if(whereColName.size() > MAX_IDENTIFIER_LENGTH){
                    int lastUnderscoreIx = whereColName.lastIndexOf('_');
                    int countToRemove = whereColName.size() - MAX_IDENTIFIER_LENGTH;
                    whereColName = QString("%1%2").arg(whereColName.left(whereColName.size()-countToRemove), whereColName.mid(lastUnderscoreIx));
                }
            }
        }

        result.append(formatBindVarName(whereColName, i + 1 + bindStartPos, bindStyle));
    }

    return result;
}

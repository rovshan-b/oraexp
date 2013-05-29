#ifndef TABLEDMLGENERATOR_H
#define TABLEDMLGENERATOR_H

#include <QString>
#include "enums.h"

class TableDmlGenerator
{
public:
    static QString generateSelectStatement(const QString &schemaName, const QString &tableName);
    static QString generateSelectStatement(const QString &schemaName, const QString &tableName, const QStringList &columns, const QStringList &whereColumns, OraExp::BindVarStyle bindStyle, bool includeSchema);
    static QString generateDeleteStatement(const QString &schemaName, const QString &tableName);
    static QString generateDeleteStatement(const QString &schemaName, const QString &tableName, const QStringList &whereColumns, OraExp::BindVarStyle bindStyle, bool includeSchema);
    static QString generateUpdateStatement(const QString &schemaName, const QString &tableName, const QStringList &columns, const QStringList &whereColumns, OraExp::BindVarStyle bindStyle, bool includeSchema);
    static QString generateInsertStatement(const QString &schemaName, const QString &tableName, const QStringList &columns, OraExp::BindVarStyle bindStyle, bool includeSchema);

    static QString generateWhereClause(const QStringList &columns, const QStringList &whereColumns, OraExp::BindVarStyle bindStyle, int bindStartPos = 0);
    static QString generateWhereBindVars(const QStringList &columns, const QStringList &whereColumns, OraExp::BindVarStyle bindStyle, int bindStartPos = 0);

private:
    TableDmlGenerator();
};

#endif // TABLEDMLGENERATOR_H

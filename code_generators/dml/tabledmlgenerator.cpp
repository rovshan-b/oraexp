#include "tabledmlgenerator.h"
#include "util/strutil.h"
#include <QStringList>

TableDmlGenerator::TableDmlGenerator()
{
}

QString TableDmlGenerator::generateDeleteStatement(const QString &schemaName, const QString &tableName)
{
    return QString("DELETE FROM \"%1\".\"%2\"").arg(schemaName, tableName);
}

QString TableDmlGenerator::generateInsertStatement(const QString &schemaName, const QString &tableName,
                                                   const QStringList &columnNames, const QStringList &bindVarNames)
{
    Q_ASSERT(columnNames.size() == bindVarNames.size());

    return QString("INSERT INTO \"%1\".\"%2\" (%3)\nVALUES(%4)").arg(schemaName, tableName, joinEnclosed(columnNames, ", "), bindVarNames.join(", "));
}

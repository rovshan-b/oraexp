#include "tabledmlgenerator.h"
#include "util/strutil.h"
#include "util/dbutil.h"
#include <QStringList>

TableDmlGenerator::TableDmlGenerator()
{
}

QString TableDmlGenerator::generateDeleteStatement(const QString &schemaName, const QString &tableName)
{
    return QString("DELETE FROM \"%1\".\"%2\"").arg(schemaName, tableName);
}

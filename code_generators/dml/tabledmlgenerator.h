#ifndef TABLEDMLGENERATOR_H
#define TABLEDMLGENERATOR_H

#include <QString>

class TableDmlGenerator
{
public:
    static QString generateDeleteStatement(const QString &schemaName, const QString &tableName);
    static QString generateInsertStatement(const QString &schemaName, const QString &tableName,
                                           const QStringList &columnNames, const QStringList &bindVarNames);

private:
    TableDmlGenerator();
};

#endif // TABLEDMLGENERATOR_H

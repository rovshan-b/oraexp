#ifndef TABLEDMLGENERATOR_H
#define TABLEDMLGENERATOR_H

#include <QString>

class TableDmlGenerator
{
public:
    static QString generateDeleteStatement(const QString &schemaName, const QString &tableName);

private:
    TableDmlGenerator();
};

#endif // TABLEDMLGENERATOR_H

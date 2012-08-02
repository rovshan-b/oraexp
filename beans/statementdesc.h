#ifndef STATEMENTDESC_H
#define STATEMENTDESC_H

#include <QString>
#include <QList>
#include <QHash>

class Param;
class Statement;
class DbConnection;

class StatementDesc
{
public:
    StatementDesc();
    ~StatementDesc();

    QString query;
    QList<Param*> params;

    void addReplacement(int paramIndex, int parentRowColIndex);

    QHash<int,int> paramReplacements;

    Statement *getStatement(DbConnection *db);
    void makeReplacements(const QStringList &parentRow);
private:
    Statement *stmt;
};

#endif // STATEMENTDESC_H

#ifndef QUERYUTIL_H
#define QUERYUTIL_H

#include <QString>
#include <QHash>

class DbConnection;

class QueryUtil
{
public:
    QueryUtil();

    static QString getQuery(const QString &queryName, DbConnection *db=0);

private:
    static QHash<QString,QString> queries;

    static QString getQueryFromFile(const QString &queryName, DbConnection *db, QStringList &replacedIncludes);
    static QString getFileNameForQuery(const QString &queryName, DbConnection *db);
    static void replacePlaceholders(QString &queryText, DbConnection *db, QStringList &replacedIncludes);
};

#endif // QUERYUTIL_H

#ifndef DBLOCALCACHE_H
#define DBLOCALCACHE_H

#include <QObject>
#include <QCache>
#include <QString>
#include <QList>
#include <QStringList>

class DbConnection;

class DbLocalCache
{
public:
    explicit DbLocalCache(DbConnection *db);

    //QStringList *getSchemaList();

private:
    QCache<QString, QStringList> cache;
    DbConnection *db;

    //QStringList *retrieveSchemaList();

};

#endif // DBLOCALCACHE_H

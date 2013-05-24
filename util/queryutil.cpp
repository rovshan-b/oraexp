#include "queryutil.h"
#include "connectivity/dbconnection.h"
#include "filesystemutil.h"

QHash<QString,QString> QueryUtil::queries;

QueryUtil::QueryUtil()
{
}

QString QueryUtil::getQuery(const QString &queryName, DbConnection *db)
{
    QString cacheKeyName=db==0 ? queryName : QString("%1_%2").arg(queryName).arg(db->getServerMajorVersion());

    if(queries.contains(cacheKeyName)){
        return queries.value(cacheKeyName);
    }else{
        QString fileText=FileSystemUtil::readTextResource(":/queries/", queryName, "sql", db);

        queries.insert(cacheKeyName, fileText);

        return fileText;
    }
}

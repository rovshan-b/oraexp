#include "dblocalcache.h"
#include "connectivity/dbconnection.h"
#include "connectivity/resultset.h"
#include <QtGui>

using namespace std;

DbLocalCache::DbLocalCache(DbConnection *db) :
     db(db)
{
}

/*
QStringList *DbLocalCache::getSchemaList()
{
    QString key="schema_list";

    QStringList *value=cache.object(key);

    if(value==0){
        value=retrieveSchemaList();
        cache.insert(key, value);
    }

    return value;
}
*/
/*QStringList *DbLocalCache::retrieveSchemaList()
{
    QStringList *schemaList=new QStringList();
    Resultset *rs=db->getNamedResultset("get_schema_list");
    QScopedPointer<Resultset> autoRs(rs);

    try{
        rs->beginFetchRows();
        while(rs->moveNext()){
            schemaList->append(rs->getString(1));
        }
        rs->endFetchRows();

        return schemaList;

    } catch(OciException&) {
        delete schemaList;
        throw;
    }
}*/

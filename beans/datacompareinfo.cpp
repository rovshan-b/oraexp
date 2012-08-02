#include "datacompareinfo.h"

DataCompareInfo::DataCompareInfo()
{
}

DataCompareInfo::DataCompareInfo(const QString &tableName,
                                 const QString &newStatus,
                                 int inserts,
                                 int updates,
                                 int deletes,
                                 const QString &dml) :
    tableName(tableName),
    newStatus(newStatus),
    inserts(inserts),
    updates(updates),
    deletes(deletes),
    dml(dml)
{
}

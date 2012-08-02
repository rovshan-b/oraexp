#ifndef TABLECOMPAREHELPER_H
#define TABLECOMPAREHELPER_H

#include "connectivity/ociexception.h"
#include "beans/tableinfo.h"
#include "dbobjectcomparehelper.h"

class TableCompareHelper : public DbObjectCompareHelper
{
    Q_OBJECT
public:
    explicit TableCompareHelper(const QString &compareResult, const QString &currentObjectName, QObject *parent);
    virtual ~TableCompareHelper();

protected:
    virtual QPair<QString,QString> generateCreateDdl(DbObjectInfo *objectInfo, const SchemaComparisonOptions *options);
    virtual QPair<QString,QString> generateDiffDdl(DbObjectInfo *srcObjectInfo, DbObjectInfo *targetObjectInfo, const SchemaComparisonOptions *options);

};

#endif // TABLECOMPAREHELPER_H

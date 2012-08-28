#ifndef DBUTIL_H
#define DBUTIL_H

#include "enums.h"
#include <QString>
#include <QStringList>
#include "navtree/dbtreemodel.h"
#include <ocilib.h>

class DbUtil
{
public:
    DbUtil();

    static void checkForOciError(OCI_Statement *stmt=0);
    static void throwOciException(OCI_Error *error, OCI_Statement *stmt);

    static QString getConstraintTypeLetter(OraExp::ConstraintType constraintType);
    static QString getConstraintTypeName(int constraintType, bool translate=true);

    static OraExp::TableType getTableType(const QString &temporary, const QString &temporaryDuration,
                                          const QString &iotType, const QString &external);

    static QString getIndexTypeName(int indexType, bool translate=true);
    static QString getIndexPartitioningTypeName(int partitioningType, bool translate=true);
    static void populatePrivilegeNames(QStringList &list);
    static QString getPartitionTypeName(OraExp::PartitionType pType);
    static QString getDbObjectTypeNameByNodeType(DbTreeModel::DbTreeNodeType nodeType);
    static QString getDbObjectIconNameByParentNodeType(DbTreeModel::DbTreeNodeType nodeType);

    static bool isLob(const QString &dataType);
    static QStringList getComparableDataTypes();

    static int getIntervalType(const QString &dataTypeName);
    static int getTimestampType(const QString &dataTypeName);

    static bool isStringType(const QString &dataTypeName);
    static bool isNumericType(const QString &dataTypeName);
    static bool isDateType(const QString &dataTypeName);
    static bool isIntervalType(const QString &dataTypeName);
    static bool isTimestampType(const QString &dataTypeName);

    static QString toIntervalOrTimestamp(const QString &columnName, const QString &dataType);
    static QString intervalOrTimestampToChar(const QString &columnName, const QString &dataType);

    static bool isPLSQLProgramUnit(DbTreeModel::DbTreeNodeType itemType);
    static DbTreeModel::DbTreeNodeType getSpecType(DbTreeModel::DbTreeNodeType itemType);
    static DbTreeModel::DbTreeNodeType getBodyType(DbTreeModel::DbTreeNodeType itemType);

    static bool isSpecType(DbTreeModel::DbTreeNodeType itemType);
};

#endif // DBUTIL_H

#ifndef DATACOMPAREINFO_H
#define DATACOMPAREINFO_H

#include <QString>

class DataCompareInfo
{
public:
    DataCompareInfo();
    DataCompareInfo(const QString &tableName, const QString &newStatus,
                    int inserts=0, int updates=0, int deletes=0,
                    const QString &dml="");

    QString tableName;
    QString newStatus;

    int inserts;
    int updates;
    int deletes;

    QString dml;
};

#endif // DATACOMPAREINFO_H

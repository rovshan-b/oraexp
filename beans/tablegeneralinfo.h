#ifndef TABLEGENERALINFO_H
#define TABLEGENERALINFO_H

#include "storageparams.h"
#include "tableadditionalattributes.h"
#include "indexorganizedtableproperties.h"
#include "tablecreateddloptions.h"
#include "defines.h"

class TableGeneralInfo
{
public:
    TableGeneralInfo();

    QString schema;
    QString tableName;
    OraExp::TableType tableType;
    StorageParams storageParams;
    TableAdditionalAttributes additionalAttributes;
    IndexOrganizedTableProperties indexOrganizedProperties;
    QString comments;

    QString getFullTableName() const;

    QString generateDdl(bool isPartitioned,
                        bool storage=true,
                        bool tablespaceOnly=false,
                        bool iotProperties=true,
                        bool attributes=true,
                        bool flashbackArchive=true) const;
    QList<NameQueryPair> generateAlterDdl(const TableGeneralInfo &originalInfo,
                                          bool tblComments=true,
                                          bool attributes=true,
                                          bool flashbackArchive=true,
                                          bool storage=true,
                                          bool iotProperties=true) const;
};

Q_DECLARE_METATYPE(TableGeneralInfo)

#endif // TABLEGENERALINFO_H

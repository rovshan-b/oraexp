#include "dbutil.h"
#include "strutil.h"
#include "iconutil.h"
#include <QObject>
#include <QtGui>
#include "defines.h"

#include <iostream>

DbUtil::DbUtil()
{
}

void DbUtil::checkForOciError(OCI_Statement *stmt)
{
    OCI_Error *err = OCI_GetLastError();
    throwOciException(err, stmt);
}

void DbUtil::throwOciException(OCI_Error *error, OCI_Statement *stmt)
{
    if(error){
        const dtext *errMsg=OCI_ErrorGetString(error);
        std::wcout << errMsg << std::endl;
        QString errorMessage=toQString(errMsg);
        int errorCode=OCI_ErrorGetOCICode(error);
        unsigned int errorPos=stmt==0 ? 0 : OCI_GetSqlErrorPos(stmt);

        if(stmt){
            OCI_Error *batchError = OCI_GetBatchError(stmt);

            int addedMessageCount = 0;
            while (batchError)
            {
                errorMessage.append(QString("\nError at row: %1. %2").
                                    arg(OCI_ErrorGetRow(batchError)).
                                    arg(toQString(OCI_ErrorGetString(batchError))));
                batchError = OCI_GetBatchError(stmt);

                if(++addedMessageCount >= 5){
                    break;
                }
            }
        }

        throw OciException(errorMessage, errorCode, -1, errorPos);
    }
}

QString DbUtil::getConstraintTypeLetter(OraExp::ConstraintType constraintType)
{
    switch(constraintType){
    case OraExp::PrimaryKey:
        return "P";
        break;
    case OraExp::ForeignKey:
        return "R";
        break;
    case OraExp::UniqueKey:
        return "U";
        break;
    default:
        return "C";
        break;
    }
}

QString DbUtil::getConstraintTypeName(int constraintType, bool translate)
{
    QString value;
    switch(constraintType)
    {
    case 0:
        value=translate ? QObject::tr("Primary key") : "Primary key";
        break;
    case 1:
        value=translate ? QObject::tr("Foreign key") : "Foreign key";
        break;
    case 2:
        value=translate ? QObject::tr("Unique") : "Unique";
        break;
    default:
        value=translate ? QObject::tr("Check") : "Check";
        break;
    }

    return value;
}

OraExp::TableType DbUtil::getTableType(const QString &temporary,
                                                      const QString &temporaryDuration,
                                                      const QString &iotType,
                                                      const QString &external)
{
    OraExp::TableType tableType=OraExp::TableTypeHeap;

    if(temporary.compare("Y")==0){
        if(temporaryDuration.compare("sys$session")){
            tableType=OraExp::TableTypeTemporarySession;
        }else{
            tableType=OraExp::TableTypeTemporaryTransaction;
        }
    }else if(!iotType.isEmpty()){
        if(iotType=="IOT"){
            tableType=OraExp::TableTypeIndexOrganized;
        }else if(iotType=="IOT_MAPPING"){
            tableType=OraExp::TableTypeIndexOrganizedMapping;
        }else{ //IOT_OVERFLOW
            tableType=OraExp::TableTypeIndexOrganizedOverflow;
        }
    }else if(external.startsWith("Y")){
        tableType=OraExp::TableTypeExternal;
    }

    return tableType;
}

QString DbUtil::getIndexTypeName(int indexType, bool translate)
{
    QString value;
    switch(indexType)
    {
    case 0:
        value=translate ? QObject::tr("Normal") : "Normal";
        break;
    case 1:
        value=translate ? QObject::tr("Bitmap") : "Bitmap";
        break;
    default:
        value=translate ? QObject::tr("Unique") : "Unique";
        break;
    }

    return value;
}

QStringList DbUtil::getIndexTypeNames()
{
    return QStringList() << QObject::tr("Normal") << QObject::tr("Bitmap") << QObject::tr("Unique");
}

QStringList DbUtil::getIndexParallelOptionNames()
{
    return QStringList() << QObject::tr("Default") << QObject::tr("Enable") << QObject::tr("Disable");
}

QString DbUtil::getIndexPartitioningTypeName(int partitioningType, bool translate)
{
    switch(partitioningType)
    {
    case 0:
        return translate? QObject::tr("None") : "None";
        break;
    case 1:
        return translate ? QObject::tr("Local") : "Local";
        break;
    default:
        return translate ? QObject::tr("Global") : "Global";
        break;
    }
}

QStringList DbUtil::getIndexPartitioningTypeNames()
{
    return QStringList() << QObject::tr("None") << QObject::tr("Local") << QObject::tr("Global");
}

void DbUtil::populatePrivilegeNames(QStringList &list, DbTreeModel::DbTreeNodeType objectType)
{
    if(objectType==DbTreeModel::Schema){
        list.append("EXECUTE");
    }

    if(objectType==DbTreeModel::Table || objectType==DbTreeModel::Schema){
        list.append("SELECT");
        list.append("INSERT");
        list.append("UPDATE");
        list.append("DELETE");
        list.append("ALTER");
        list.append("INDEX");
        list.append("REFERENCES");
        list.append("DEBUG");
    }

    if(objectType==DbTreeModel::Schema){
        list.append("UNDER");
        list.append("READ");
        list.append("WRITE");
    }
}

QString DbUtil::getPartitionTypeName(OraExp::PartitionType pType)
{
    QString result;

    switch(pType){
    case OraExp::PartitionTypeRange:
        result = "RANGE";
        break;
    case OraExp::PartitionTypeList:
        result = "LIST";
        break;
    case OraExp::PartitionTypeHash:
        result = "HASH";
        break;
    case OraExp::PartitionTypeReference:
        result = "REFERENCE";
        break;
    default:
        qDebug() << "unknown partition type in TableCreatorPartitionsTab::getPartitionTypeName";
        result = "UNKNOWN";
        break;
    }

    return result;
}

QStringList DbUtil::getConstraintTypeNames()
{
    return QStringList() << QObject::tr("Primary key") << QObject::tr("Foreign key") << QObject::tr("Unique");
}

QList<QPixmap> DbUtil::getConstraintTypeIcons()
{
    return QList<QPixmap>() << IconUtil::getIcon("column_pk") << IconUtil::getIcon("column_fk") << IconUtil::getIcon("column_uq");
}

QStringList DbUtil::getOnDeleteActionNames()
{
    return QStringList() << QObject::tr("No action") << QObject::tr("Cascade") << QObject::tr("Set null");
}

QStringList DbUtil::getOracleLoaderDataTypeNames()
{
    QStringList dataTypeList;

    dataTypeList.append("");
    dataTypeList.append("CHAR(255)");
    dataTypeList.append("INTEGER");
    dataTypeList.append("INTEGER EXTERNAL");
    dataTypeList.append("UNSIGNED INTEGER");
    dataTypeList.append("UNSIGNED INTEGER EXTERNAL");
    dataTypeList.append("DECIMAL");
    dataTypeList.append("DECIMAL EXTERNAL");
    dataTypeList.append("ZONED");
    dataTypeList.append("ZONED EXTERNAL");
    dataTypeList.append("ORACLE_DATE");
    dataTypeList.append("ORACLE_NUMBER");
    dataTypeList.append("ORACLE_NUMBER COUNTED");
    dataTypeList.append("FLOAT");
    dataTypeList.append("FLOAT EXTERNAL");
    dataTypeList.append("DOUBLE");
    dataTypeList.append("BINARY FLOAT");
    dataTypeList.append("BINARY FLOAT EXTERNAL");
    dataTypeList.append("BINARY_DOUBLE");
    dataTypeList.append("RAW");
    dataTypeList.append("VARCHAR(255)");
    dataTypeList.append("VARRAW(255)");
    dataTypeList.append("VARCHARC(255)");
    dataTypeList.append("VARRAWC(255)");

    return dataTypeList;
}

QStringList DbUtil::getOracleLoaderTrimTypes()
{
    QStringList list;

    list.append("");
    list.append("LRTRIM");
    list.append("LTRIM");
    list.append("RTRIM");
    list.append("LDRTRIM");
    list.append("NOTRIM");

    return list;
}

QStringList DbUtil::getDateTypeNames()
{
    QStringList dateTypes;

    dateTypes.append("");
    dateTypes.append("DATE");
    dateTypes.append("TIMESTAMP WITH TIME ZONE");
    dateTypes.append("TIMESTAMP WITH LOCAL TIME ZONE");
    dateTypes.append("INTERVAL YEAR_TO_MONTH");
    dateTypes.append("INTERVAL DAY_TO_SECOND");

    return dateTypes;
}

QStringList DbUtil::getExternalTableColumnTransformTypes()
{
    QStringList list;

    list.append("");
    list.append("NULL");
    list.append("CONSTANT");
    list.append("CONCAT");
    list.append("LOBFILE");

    return list;
}

QStringList DbUtil::getExternalTableLobTypeNames()
{
    return QStringList () << "" << "CLOB" << "BLOB";
}

QStringList DbUtil::getGrantTypes()
{
    QStringList grantTypes;

    grantTypes.append("");
    grantTypes.append(QObject::tr("GRANT"));
    grantTypes.append(QObject::tr("WITH GRANT OPTION"));

    return grantTypes;
}

QString DbUtil::getDbObjectDisplayNameByNodeType(DbTreeModel::DbTreeNodeType nodeType)
{
    QString result;

    switch(nodeType){
    case DbTreeModel::Tables:
    case DbTreeModel::Table:
        result=QObject::tr("Tables");
        break;
    case DbTreeModel::Views:
    case DbTreeModel::View:
        result=QObject::tr("Views");
        break;
    case DbTreeModel::Packages:
    case DbTreeModel::Package:
        result=QObject::tr("Packages");
        break;
    case DbTreeModel::PackageSpecs:
    case DbTreeModel::PackageSpec:
        result=QObject::tr("Package specs");
        break;
    case DbTreeModel::PackageBodies:
    case DbTreeModel::PackageBody:
        result=QObject::tr("Package bodies");
        break;
    case DbTreeModel::Procedures:
    case DbTreeModel::Procedure:
        result=QObject::tr("Procedures");
        break;
    case DbTreeModel::Functions:
    case DbTreeModel::Function:
        result=QObject::tr("Functions");
        break;
    case DbTreeModel::SchemaTriggers:
    case DbTreeModel::Triggers:
    case DbTreeModel::Trigger:
        result=QObject::tr("Triggers");
        break;
    case DbTreeModel::SchemaIndexes:
    case DbTreeModel::Indexes:
    case DbTreeModel::Index:
        result=QObject::tr("Indexes");
        break;
    case DbTreeModel::Sequences:
    case DbTreeModel::Sequence:
        result=QObject::tr("Sequences");
        break;
    case DbTreeModel::TypeSpecs:
    case DbTreeModel::TypeSpec:
        result=QObject::tr("Type specs");
        break;
    case DbTreeModel::Types:
    case DbTreeModel::Type:
        result=QObject::tr("Types");
        break;
    case DbTreeModel::TypeBodies:
    case DbTreeModel::TypeBody:
        result=QObject::tr("Type bodies");
        break;
    case DbTreeModel::Synonyms:
    case DbTreeModel::Synonym:
        result=QObject::tr("Synonyms");
        break;
    case DbTreeModel::PublicSynonyms:
        result=QObject::tr("Public synonyms");
        break;
    case DbTreeModel::DatabaseLinks:
    case DbTreeModel::DatabaseLink:
        result=QObject::tr("Database links");
        break;
    case DbTreeModel::PublicDatabaseLinks:
        result=QObject::tr("Public database links");
        break;
    case DbTreeModel::AllSchemas:
        result=QObject::tr("All schemas");
        break;
    case DbTreeModel::Schema:
        result=QObject::tr("Schemas");
        break;
    default:
        Q_ASSERT(false);
        result=QObject::tr("Unsupported type");
        break;
    }

    return result;
}

QString DbUtil::getDbObjectTypeNameByNodeType(DbTreeModel::DbTreeNodeType nodeType)
{
    QString result;

    switch(nodeType){
    case DbTreeModel::Tables:
    case DbTreeModel::Table:
        result="TABLE";
        break;
    case DbTreeModel::Views:
    case DbTreeModel::View:
        result="VIEW";
        break;
    case DbTreeModel::Packages:
    case DbTreeModel::Package:
        result="PACKAGE";
        break;
    case DbTreeModel::PackageSpecs:
    case DbTreeModel::PackageSpec:
        result="PACKAGE SPEC";
        break;
    case DbTreeModel::PackageBodies:
    case DbTreeModel::PackageBody:
        result="PACKAGE BODY";
        break;
    case DbTreeModel::Procedures:
    case DbTreeModel::Procedure:
        result="PROCEDURE";
        break;
    case DbTreeModel::Functions:
    case DbTreeModel::Function:
        result="FUNCTION";
        break;
    case DbTreeModel::SchemaTriggers:
    case DbTreeModel::Triggers:
    case DbTreeModel::Trigger:
        result="TRIGGER";
        break;
    case DbTreeModel::SchemaIndexes:
    case DbTreeModel::Indexes:
    case DbTreeModel::Index:
        result="INDEX";
        break;
    case DbTreeModel::Sequences:
    case DbTreeModel::Sequence:
        result="SEQUENCE";
        break;
    case DbTreeModel::TypeSpecs:
    case DbTreeModel::TypeSpec:
        result="TYPE SPEC";
        break;
    case DbTreeModel::Types:
    case DbTreeModel::Type:
        result="TYPE";
        break;
    case DbTreeModel::TypeBodies:
    case DbTreeModel::TypeBody:
        result="TYPE BODY";
        break;
    case DbTreeModel::Synonyms:
    case DbTreeModel::Synonym:
        result="SYNONYM";
        break;
    case DbTreeModel::DatabaseLinks:
    case DbTreeModel::PublicDatabaseLinks:
    case DbTreeModel::DatabaseLink:
        result="DB_LINK";
        break;
    case DbTreeModel::AllSchemas:
    case DbTreeModel::Schema:
        result="USER";
        break;
    default:
        result="UNSUPPORTED_TYPE";
        break;
    }

    return result;
}

DbTreeModel::DbTreeNodeType DbUtil::getDbObjectParentNodeType(DbTreeModel::DbTreeNodeType nodeType)
{
    DbTreeModel::DbTreeNodeType result;

    switch(nodeType){
    case DbTreeModel::Table:
        result=DbTreeModel::Tables;
        break;
    case DbTreeModel::Column:
        result=DbTreeModel::Columns;
        break;
    case DbTreeModel::Constraint:
        result=DbTreeModel::Constraints;
        break;
    case DbTreeModel::View:
        result=DbTreeModel::Views;
        break;
    case DbTreeModel::Package:
    case DbTreeModel::PackageSpec:
    case DbTreeModel::PackageBody:
        result=DbTreeModel::Packages;
        break;
    case DbTreeModel::Procedure:
        result=DbTreeModel::Procedures;
        break;
    case DbTreeModel::Function:
        result=DbTreeModel::Functions;
        break;
    case DbTreeModel::Trigger:
        result=DbTreeModel::SchemaTriggers;
        break;
    case DbTreeModel::Index:
        result=DbTreeModel::SchemaIndexes;
        break;
    case DbTreeModel::Sequence:
        result=DbTreeModel::Sequences;
        break;
    case DbTreeModel::Type:
    case DbTreeModel::TypeSpec:
    case DbTreeModel::TypeBody:
        result=DbTreeModel::Types;
        break;
    case DbTreeModel::Synonym:
        result=DbTreeModel::Synonyms;
        break;
    case DbTreeModel::DatabaseLink:
        result=DbTreeModel::DatabaseLinks;
        break;
    case DbTreeModel::Schema:
        result=DbTreeModel::AllSchemas;
        break;
    default:
        result=DbTreeModel::Unknown;
        break;
    }

    return result;
}

DbTreeModel::DbTreeNodeType DbUtil::getDbObjectNodeTypeByTypeName(const QString &typeName)
{
    DbTreeModel::DbTreeNodeType result=DbTreeModel::Unknown;

    if(typeName=="TABLE"){
        result = DbTreeModel::Table;
    }else if(typeName=="COLUMN"){
        result = DbTreeModel::Column;
    }else if(typeName=="CONSTRAINT"){
        result = DbTreeModel::Constraint;
    }else if(typeName=="VIEW"){
        result = DbTreeModel::View;
    }else if(typeName=="PACKAGE"){
        result = DbTreeModel::Package;
    }else if(typeName=="PACKAGE SPEC"){
        result = DbTreeModel::PackageSpec;
    }else if(typeName=="PACKAGE BODY"){
        result = DbTreeModel::PackageBody;
    }else if(typeName=="PROCEDURE"){
        result = DbTreeModel::Procedure;
    }else if(typeName=="FUNCTION"){
        result = DbTreeModel::Function;
    }else if(typeName=="TRIGGER"){
        result = DbTreeModel::Trigger;
    }else if(typeName=="INDEX"){
        result = DbTreeModel::Index;
    }else if(typeName=="SEQUENCE"){
        result = DbTreeModel::Sequence;
    }else if(typeName=="TYPE"){
        result = DbTreeModel::Type;
    }else if(typeName=="TYPE SPEC"){
        result = DbTreeModel::TypeSpec;
    }else if(typeName=="TYPE BODY"){
        result = DbTreeModel::TypeBody;
    }else if(typeName=="SYNONYM"){
        result = DbTreeModel::Synonym;
    }else if(typeName=="DB_LINK"){
        result = DbTreeModel::DatabaseLink;
    }else if(typeName=="USER" || typeName=="SCHEMA"){
        result = DbTreeModel::Schema;
    }

    return result;
}

QString DbUtil::getDbObjectIconNameByParentNodeType(DbTreeModel::DbTreeNodeType nodeType)
{
    QString result;

    switch(nodeType){
    case DbTreeModel::Tables:
    case DbTreeModel::Table:
        result="table";
        break;
    case DbTreeModel::Views:
    case DbTreeModel::View:
        result="view";
        break;
    case DbTreeModel::Packages:
    case DbTreeModel::PackageSpecs:
    case DbTreeModel::PackageSpec:
    case DbTreeModel::PackageBodies:
    case DbTreeModel::PackageBody:
    case DbTreeModel::Package:
        result="package";
        break;
    case DbTreeModel::Procedures:
    case DbTreeModel::Procedure:
        result="procedure";
        break;
    case DbTreeModel::Functions:
    case DbTreeModel::Function:
        result="function";
        break;
    case DbTreeModel::SchemaTriggers:
    case DbTreeModel::Triggers:
    case DbTreeModel::Trigger:
        result="trigger";
        break;
    case DbTreeModel::SchemaIndexes:
    case DbTreeModel::Indexes:
    case DbTreeModel::Index:
        result="index";
        break;
    case DbTreeModel::Sequences:
    case DbTreeModel::Sequence:
        result="sequence";
        break;
    case DbTreeModel::Types:
    case DbTreeModel::TypeSpecs:
    case DbTreeModel::TypeBodies:
    case DbTreeModel::Type:
    case DbTreeModel::TypeSpec:
    case DbTreeModel::TypeBody:
        result="type";
        break;
    case DbTreeModel::Synonyms:
    case DbTreeModel::Synonym:
        result="synonym";
        break;
    case DbTreeModel::DatabaseLinks:
    case DbTreeModel::PublicDatabaseLinks:
    case DbTreeModel::DatabaseLink:
        result="dblink";
        break;
    case DbTreeModel::AllSchemas:
    case DbTreeModel::Schema:
        result="user";
        break;
    default:
        result="UNSUPPORTED_TYPE";
        break;
    }

    return result;
}

QString DbUtil::getTableIconName(bool isPk)
{
    return isPk ? "column_pk" : "column";
}

QString DbUtil::getSizeUnitText(OraExp::ExtentUnit unit)
{
    QString res;
    switch(unit){
    case OraExp::Bytes:
        res="";
        break;
    case OraExp::KB:
        res="K";
        break;
    case OraExp::MB:
        res="M";
        break;
    default:
        res="G";
        break;
    }

    return res;
}

OraExp::ExtentUnit DbUtil::getSizeUnitFromText(const QString &unit)
{
    OraExp::ExtentUnit res;

    if(unit.isEmpty() || unit=="B"){
        res=OraExp::Bytes;
    }else if(unit=="K"){
        res=OraExp::KB;
    }else if(unit=="M"){
        res=OraExp::MB;
    }else{ //unit=="G"
        res=OraExp::GB;
    }

    return res;
}

QStringList DbUtil::getSizeUnitTexts()
{
    return QStringList() <<
           QObject::tr("Bytes") <<
           QObject::tr("KB") <<
           QObject::tr("MB") <<
           QObject::tr("GB");
}

bool DbUtil::isLob(const QString &dataType)
{
    if(dataType.compare("BLOB", Qt::CaseInsensitive) == 0 ||
            dataType.compare("CLOB", Qt::CaseInsensitive) ==0 ||
            dataType.compare("NCLOB", Qt::CaseInsensitive)==0 ){
        return true;
    }

    return false;
}

QStringList DbUtil::getComparableDataTypes()
{
    QStringList comparableDataTypes;

    comparableDataTypes << "VARCHAR2" << "NVARCHAR2" << "NUMBER" << "FLOAT" <<
                          "BINARY_FLOAT" << "BINARY_DOUBLE" << "CHAR" << "NCHAR" << "RAW" <<
                           "DATE" << "TIMESTAMP\\(\\d+\\)" << "TIMESTAMP\\(\\d+\\) WITH TIME ZONE" <<
                           "TIMESTAMP\\(\\d+\\) WITH LOCAL TIME ZONE" <<
                          "INTERVAL YEAR(\\(\\d+\\))? TO MONTH(\\(\\d+\\))?" <<
                          "INTERVAL DAY(\\(\\d+\\))? TO SECOND(\\(\\d+\\))?";

    return comparableDataTypes;
}

OraExp::ColumnSubType DbUtil::getIntervalSubType(const QString &dataTypeName)
{
    if(dataTypeName.contains("SECOND")){
        return OraExp::CSTIntervalDs;
    }else{
        return OraExp::CSTIntervalYm;
    }
}

OraExp::ColumnSubType DbUtil::getTimestampSubType(const QString &dataTypeName)
{
    if(dataTypeName.endsWith("LOCAL TIME ZONE")){
        return OraExp::CSTTimestampLtz;
    }else if(dataTypeName.endsWith("TIME ZONE")){
        return OraExp::CSTTimestampTz;
    }else{
        return OraExp::CSTTimestamp;
    }
}

unsigned int DbUtil::toOciTimestampSubType(OraExp::ColumnSubType tsSubType)
{
    unsigned int ociTsSubType;

    switch(tsSubType){
    case OraExp::CSTTimestamp:
        ociTsSubType=OCI_TIMESTAMP;
        break;
    case OraExp::CSTTimestampTz:
        ociTsSubType=OCI_TIMESTAMP_TZ;
        break;
    case OraExp::CSTTimestampLtz:
        ociTsSubType=OCI_TIMESTAMP_LTZ;
        break;
    default:
        ociTsSubType=OCI_UNKNOWN;
        Q_ASSERT(false);
        break;
    }

    return ociTsSubType;
}

unsigned int DbUtil::toOciIntervalSubType(OraExp::ColumnSubType intSubType)
{
    unsigned int ociIntSubType;

    switch(intSubType){
    case OraExp::CSTIntervalDs:
        ociIntSubType=OCI_INTERVAL_DS;
    case OraExp::CSTIntervalYm:
        ociIntSubType=OCI_INTERVAL_YM;
    default:
        ociIntSubType=OCI_UNKNOWN;
        Q_ASSERT(false);
        break;
    }

    return ociIntSubType;
}

bool DbUtil::isStringType(const QString &dataTypeName)
{
    return dataTypeName=="VARCHAR2" || dataTypeName=="NVARCHAR2" ||
            dataTypeName=="CHAR" || dataTypeName=="NCHAR";
}

bool DbUtil::isRawType(const QString &dataTypeName)
{
    return dataTypeName=="RAW";
}

bool DbUtil::isNumericType(const QString &dataTypeName)
{
    return dataTypeName=="NUMBER" || dataTypeName=="FLOAT" ||
            dataTypeName=="BINARY_FLOAT" || dataTypeName=="BINARY_DOUBLE";
}

bool DbUtil::isNumericType(OraExp::ColumnDataType dataType)
{
    return dataType==OraExp::CDTNumeric;
}

bool DbUtil::isDateType(const QString &dataTypeName)
{
    return dataTypeName=="DATE";
}

bool DbUtil::isDateType(OraExp::ColumnDataType dataType)
{
    return dataType==OraExp::CDTDateTime;
}

bool DbUtil::isIntervalType(const QString &dataTypeName)
{
    return dataTypeName.startsWith("INTERVAL");
}

bool DbUtil::isIntervalType(OraExp::ColumnDataType dataType)
{
    return dataType==OraExp::CDTInterval;
}

bool DbUtil::isTimestampType(const QString &dataTypeName)
{
    return dataTypeName.startsWith("TIMESTAMP");
}

bool DbUtil::isTimestampType(OraExp::ColumnDataType dataType)
{
    return dataType==OraExp::CDTTimestamp;
}

QString DbUtil::toDate(const QString &columnName, const QString &format)
{
    return QString("to_date(%1,'%2')").arg(columnName, format.isEmpty() ? DB_DATE_FORMAT : format);
}

QString DbUtil::toInterval(const QString &columnName, OraExp::ColumnSubType intSubType)
{
    QString result;

    if(intSubType==OraExp::CSTIntervalDs){
        result = QString("to_dsinterval(%1)").arg(columnName);
    }else{
        result = QString("to_yminterval(%1)").arg(columnName);
    }

    return result;
}

QString DbUtil::toTimestamp(const QString &columnName, OraExp::ColumnSubType tsSubType,
                            const QString &format)
{
    QString result;

    if(tsSubType==OraExp::CSTTimestampTz){
        result = QString("to_timestamp_tz(%1,'%2')").arg(columnName, format.isEmpty() ? DB_TZ_TIMESTAMP_FORMAT : format);
    }else{
        result = QString("to_timestamp(%1,'%2')").arg(columnName, format.isEmpty() ? DB_TIMESTAMP_FORMAT : format);
    }

    return result;
}

QString DbUtil::toDateOrIntervalOrTimestamp(const QString &columnName, const QString &dataType, const QString &format)
{
    if(DbUtil::isDateType(dataType)){
        return DbUtil::toDate(columnName, format);
    }else{
        return DbUtil::toIntervalOrTimestamp(columnName, dataType, format);
    }
}

QString DbUtil::toIntervalOrTimestamp(const QString &columnName, const QString &dataType, const QString &format)
{
    QString result = columnName;
    if(DbUtil::isIntervalType(dataType)){
        OraExp::ColumnSubType intervalSubType=DbUtil::getIntervalSubType(dataType);
        result = toInterval(columnName, intervalSubType);
    }else if(DbUtil::isTimestampType(dataType)){
        OraExp::ColumnSubType timestampSubType=DbUtil::getTimestampSubType(dataType);
        result = toTimestamp(columnName, timestampSubType, format);
    }
    return result;
}

QString DbUtil::intervalOrTimestampToChar(const QString &columnName, const QString &dataType)
{
    QString result = columnName;
    if(DbUtil::isIntervalType(dataType)){
        result = QString("to_char(%1)").arg(columnName);
    }else if(DbUtil::isTimestampType(dataType)){
        OraExp::ColumnSubType timestampSubType=DbUtil::getTimestampSubType(dataType);
        if(timestampSubType==OraExp::CSTTimestampTz){
            result = QString("to_char(%1,'%2')").arg(columnName, DB_TZ_TIMESTAMP_FORMAT);
        }else{
            result = QString("to_char(%1,'%2')").arg(columnName, DB_TIMESTAMP_FORMAT);
        }
    }
    return result;
}

bool DbUtil::isPLSQLProgramUnit(DbTreeModel::DbTreeNodeType itemType)
{
    return itemType==DbTreeModel::PackageSpec ||
            itemType==DbTreeModel::PackageBody ||
            itemType==DbTreeModel::Procedure ||
            itemType==DbTreeModel::Function ||
            itemType==DbTreeModel::Trigger ||
            itemType==DbTreeModel::TypeSpec ||
            itemType==DbTreeModel::TypeBody;
}

bool DbUtil::isPLSQLProgramUnit(const QString &itemType)
{
    return itemType.compare("PACKAGE", Qt::CaseInsensitive)==0 ||
            itemType.compare("PACKAGE SPEC", Qt::CaseInsensitive)==0 ||
            itemType.compare("PACKAGE BODY", Qt::CaseInsensitive)==0 ||
            itemType.compare("PROCEDURE", Qt::CaseInsensitive)==0 ||
            itemType.compare("FUNCTION", Qt::CaseInsensitive)==0 ||
            itemType.compare("TRIGGER", Qt::CaseInsensitive)==0 ||
            itemType.compare("TYPE", Qt::CaseInsensitive)==0 ||
            itemType.compare("TYPE SPEC", Qt::CaseInsensitive)==0 ||
            itemType.compare("TYPE BODY", Qt::CaseInsensitive)==0 ||
            itemType.compare("JAVA", Qt::CaseInsensitive)==0;
}

DbTreeModel::DbTreeNodeType DbUtil::getSpecType(DbTreeModel::DbTreeNodeType itemType)
{
    switch(itemType){
    case DbTreeModel::Packages:
    case DbTreeModel::Package:
    case DbTreeModel::PackageSpecs:
    case DbTreeModel::PackageSpec:
    case DbTreeModel::PackageBodies:
    case DbTreeModel::PackageBody:
        return DbTreeModel::PackageSpec;
        break;
    case DbTreeModel::Types:
    case DbTreeModel::Type:
    case DbTreeModel::TypeSpecs:
    case DbTreeModel::TypeSpec:
    case DbTreeModel::TypeBodies:
    case DbTreeModel::TypeBody:
        return DbTreeModel::TypeSpec;
        break;
    default:
        Q_ASSERT(false);
        return DbTreeModel::Unknown;
        break;
    }
}

DbTreeModel::DbTreeNodeType DbUtil::getBodyType(DbTreeModel::DbTreeNodeType itemType)
{
    switch(itemType){
    case DbTreeModel::Packages:
    case DbTreeModel::Package:
    case DbTreeModel::PackageSpecs:
    case DbTreeModel::PackageSpec:
    case DbTreeModel::PackageBodies:
    case DbTreeModel::PackageBody:
        return DbTreeModel::PackageBody;
        break;
    case DbTreeModel::Types:
    case DbTreeModel::Type:
    case DbTreeModel::TypeSpecs:
    case DbTreeModel::TypeSpec:
    case DbTreeModel::TypeBodies:
    case DbTreeModel::TypeBody:
        return DbTreeModel::TypeBody;
        break;
    default:
        Q_ASSERT(false);
        return DbTreeModel::Unknown;
        break;
    }
}

bool DbUtil::isSpecType(DbTreeModel::DbTreeNodeType itemType)
{
    switch(itemType){
    case DbTreeModel::Packages:
    case DbTreeModel::Package:
    case DbTreeModel::PackageSpecs:
    case DbTreeModel::PackageSpec:
    case DbTreeModel::Types:
    case DbTreeModel::Type:
    case DbTreeModel::TypeSpecs:
    case DbTreeModel::TypeSpec:
        return true;
        break;
    default:
        return false;
        break;
    }
}

QString DbUtil::getExtentSizeClause(bool unlimited, uint size, OraExp::ExtentUnit sizeUnit)
{
    return (unlimited ? "UNLIMITED" : QString("%1%2").arg(QString::number(size), DbUtil::getSizeUnitText(sizeUnit)));
}

void DbUtil::parseExtentSize(const QString &textToParse, bool *unlimited, qulonglong *quota, OraExp::ExtentUnit *units)
{
    if(textToParse.trimmed().isEmpty()){
        *unlimited=false;
        *quota=0;
        *units=OraExp::Bytes;
        return;
    }

    if(textToParse.trimmed().compare("UNLIMITED", Qt::CaseInsensitive)==0){
        *unlimited=true;
        return;
    }

    QRegExp sizeRegExp("(\\d+)(\\w?)");
    bool isExactMatch=sizeRegExp.exactMatch(textToParse);
    Q_ASSERT(isExactMatch);
    QStringList matches=sizeRegExp.capturedTexts();
    Q_ASSERT(matches.size()==2 || matches.size()==3);

    *quota=matches.at(1).toULongLong();

    if(matches.size()==3){
        *units=DbUtil::getSizeUnitFromText(matches.at(2));
    }else{
        *units=OraExp::Bytes;
    }
}

void DbUtil::getDateFormats(QList<QString> *dateFormats, QList<QString> *datePatterns)
{
    datePatterns->append("\\d{4}(/|-|.)\\d{2}(/|-|.)\\d{2}");
    dateFormats->append("YYYY-MM-DD");

    datePatterns->append("\\d{2}(/|-|.)\\d{2}(/|-|.)\\d{4}");
    dateFormats->append("DD-MM-YYYY");

    datePatterns->append("\\d{2}(/|-|.| )\\D{3}(/|-|.| )\\d{4}");
    dateFormats->append("DD-MON-YYYY");

    datePatterns->append("\\d{2}(/|-|.| )\\D{4,}(/|-|.| )\\d{4}");
    dateFormats->append("DD-MONTH-YYYY");
}

void DbUtil::getTimeFormats(QList<QString> *timeFormats, QList<QString> *timePatterns)
{
    timePatterns->append("\\d{2}:\\d{2}");
    timeFormats->append("HH24:MI");

    timePatterns->append("\\d{2}:\\d{2}:\\d{2}");
    timeFormats->append("HH24:MI:SS");

    timePatterns->append("\\d{2}:\\d{2} (am|pm)");
    timeFormats->append("HH:MI AM");

    timePatterns->append("\\d{2}:\\d{2}:\\d{2} (am|pm)");
    timeFormats->append("HH:MI:SS AM");
}

QStringList DbUtil::getDateTimeFormats()
{
    QList<QString> datePatterns;
    QList<QString> dateFormats;

    QList<QString> timePatterns;
    QList<QString> timeFormats;

    DbUtil::getDateFormats(&dateFormats, &datePatterns);
    DbUtil::getTimeFormats(&timeFormats, &timePatterns);

    dateFormats.insert(2, "MM-DD-YYYY");

    timeFormats.append("HH24:MI:SS.FF9");
    timeFormats.append("HH24:MI:SS.FF9TZH:TZM");

    QStringList results;

    for(int i=0; i<dateFormats.size(); ++i){

        results.append(dateFormats.at(i));

        for(int k=0; k<timeFormats.size(); ++k){
            results.append(QString("%1 %2").arg(dateFormats.at(i), timeFormats.at(k)));
        }
    }

    return results;
}

QString DbUtil::getSaveSuffixForItemType(DbTreeModel::DbTreeNodeType itemType)
{
    QString result;

    switch(itemType){
    case DbTreeModel::PackageSpec:
        result = "pks";
        break;
    case DbTreeModel::PackageBody:
        result = "pkb";
        break;
    case DbTreeModel::TypeSpec:
        result = "ts";
        break;
    case DbTreeModel::TypeBody:
        result = "tb";
        break;
    case DbTreeModel::Procedure:
        result = "prc";
        break;
    case DbTreeModel::Function:
        result = "fnc";
        break;
    case DbTreeModel::Trigger:
        result = "trg";
        break;
    default:
        result = "sql";
        break;
    }

    return result;
}

QList<EnvironmentInfo> DbUtil::getEnvironmentList()
{
    EnvironmentInfo production;
    production.title = QObject::tr("Production");
    production.icon = IconUtil::getEnvironmentIcon(OraExp::Production);
    production.environment = OraExp::Production;

    EnvironmentInfo staging;
    staging.title = QObject::tr("Staging");
    staging.icon = IconUtil::getEnvironmentIcon(OraExp::Staging);
    staging.environment = OraExp::Staging;

    EnvironmentInfo development;
    development.title = QObject::tr("Development");
    development.icon = IconUtil::getEnvironmentIcon(OraExp::Development);
    development.environment = OraExp::Development;

    EnvironmentInfo test;
    test.title = QObject::tr("Test");
    test.icon = IconUtil::getEnvironmentIcon(OraExp::Test);
    test.environment = OraExp::Test;

    return QList<EnvironmentInfo>() << production << staging << development << test;
}

/*
DbTreeModel::DbTreeNodeType DbUtil::getPairNodeType(DbTreeModel::DbTreeNodeType nodeType)
{
    DbTreeModel::DbTreeNodeType result;

    if(nodeType==DbTreeModel::PackageSpec){
        result = DbTreeModel::PackageBody;
    }else if(nodeType==DbTreeModel::PackageBody){
        result = DbTreeModel::PackageSpec;
    }else if(nodeType==DbTreeModel::TypeSpec){
        result = DbTreeModel::TypeBody;
    }else if(nodeType==DbTreeModel::TypeBody){
        result = DbTreeModel::TypeSpec;
    }else{
        result = DbTreeModel::Unknown;
        Q_ASSERT(false);
    }

    return result;
}*/

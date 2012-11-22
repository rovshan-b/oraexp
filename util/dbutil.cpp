#include "dbutil.h"
#include "strutil.h"
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
        unsigned int errorRow=OCI_ErrorGetRow(error);
        unsigned int errorPos=stmt==0 ? 0 : OCI_GetSqlErrorPos(stmt);

        throw OciException(errorMessage, errorCode, errorRow, errorPos);
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
        result="DATABASE LINK";
        break;
    case DbTreeModel::AllSchemas:
    case DbTreeModel::Schema:
        result="USER";
        break;
    default:
        Q_ASSERT(false);
        result="UNSUPPORTED_TYPE";
        break;
    }

    return result;
}

DbTreeModel::DbTreeNodeType DbUtil::getDbObjectNodeTypeByTypeName(const QString &typeName)
{
    DbTreeModel::DbTreeNodeType result=DbTreeModel::Unknown;

    if(typeName=="TABLE"){
        result = DbTreeModel::Table;
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
    }else if(typeName=="DATABASE LINK"){
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
        Q_ASSERT(false);
        result="UNSUPPORTED_TYPE";
        break;
    }

    return result;
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

int DbUtil::getIntervalType(const QString &dataTypeName)
{
    if(dataTypeName.contains("SECOND")){
        return OCI_INTERVAL_DS;
    }else{
        return OCI_INTERVAL_YM;
    }
}

int DbUtil::getTimestampType(const QString &dataTypeName)
{
    if(dataTypeName.endsWith("LOCAL TIME ZONE")){
        return OCI_TIMESTAMP_LTZ;
    }else if(dataTypeName.endsWith("TIME ZONE")){
        return OCI_TIMESTAMP_TZ;
    }else{
        return OCI_TIMESTAMP;
    }
}

bool DbUtil::isStringType(const QString &dataTypeName)
{
    return dataTypeName=="VARCHAR2" || dataTypeName=="NVARCHAR2" ||
            dataTypeName=="RAW" || dataTypeName=="CHAR" || dataTypeName=="NCHAR";
}

bool DbUtil::isNumericType(const QString &dataTypeName)
{
    return dataTypeName=="NUMBER" || dataTypeName=="FLOAT" ||
            dataTypeName=="BINARY_FLOAT" || dataTypeName=="BINARY_DOUBLE";
}

bool DbUtil::isDateType(const QString &dataTypeName)
{
    return dataTypeName=="DATE";
}

bool DbUtil::isIntervalType(const QString &dataTypeName)
{
    return dataTypeName.startsWith("INTERVAL");
}

bool DbUtil::isTimestampType(const QString &dataTypeName)
{
    return dataTypeName.startsWith("TIMESTAMP");
}

QString DbUtil::toIntervalOrTimestamp(const QString &columnName, const QString &dataType)
{
    QString result = columnName;
    if(DbUtil::isIntervalType(dataType)){
        int intervalType=DbUtil::getIntervalType(dataType);
        if(intervalType==OCI_INTERVAL_DS){
            result = QString("to_dsinterval(%1)").arg(columnName);
        }else{
            result = QString("to_yminterval(%1)").arg(columnName);
        }
    }else if(DbUtil::isTimestampType(dataType)){
        int timestampType=DbUtil::getTimestampType(dataType);
        if(timestampType==OCI_TIMESTAMP_TZ){
            result = QString("to_timestamp_tz(%1,'%2')").arg(columnName, DB_TZ_TIMESTAMP_FORMAT);
        }else{
            result = QString("to_timestamp(%1,'%2')").arg(columnName, DB_TIMESTAMP_FORMAT);
        }
    }
    return result;
}

QString DbUtil::intervalOrTimestampToChar(const QString &columnName, const QString &dataType)
{
    QString result = columnName;
    if(DbUtil::isIntervalType(dataType)){
        result = QString("to_char(%1)").arg(columnName);
    }else if(DbUtil::isTimestampType(dataType)){
        int timestampType=DbUtil::getTimestampType(dataType);
        if(timestampType==OCI_TIMESTAMP_TZ){
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

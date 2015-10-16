#include "resultset.h"
#include "connection.h"
#include "statement.h"
#include <iostream>
#include "util/strutil.h"
#include "util/dbutil.h"
#include "defines.h"
#include <QThread>
#include <QDebug>

using namespace std;

#ifdef DEBUG
QAtomicInt Resultset::objectCount;
#endif

Resultset::Resultset(OCI_Resultset *ociResultset, Connection *cn, Statement *stmt) :
    cn(cn), stmt(stmt),
    columnMetadata(new ResultsetColumnMetadata()),
    reachedEOF(false), acquiredMutex(false)
{
    this->ociResultset=ociResultset;

    ResultsetColumnMetadata *metadata=columnMetadata.data();

    unsigned int colCount = OCI_GetColumnCount(ociResultset);
    OCI_Column *column;
    QString columnName;
    metadata->columnTitles.reserve(colCount);
    for(unsigned int i=1; i<=colCount; ++i){
        column=OCI_GetColumn(ociResultset, i);
        columnName=toQString(OCI_GetColumnName(column));
        if(metadata->columnTitles.contains(columnName)){
            columnName = addNumericSuffix(columnName, metadata->columnTitles);
        }

        metadata->columnTitles.append(columnName);
        metadata->columnDataTypes.insert(i, convertColumnDataType(OCI_GetColumnType(column)));
        metadata->columnSubTypes.insert(i, convertColumnSubType(i, OCI_ColumnGetSubType(column)));

        if(OCI_ColumnGetCharsetForm(column)!=OCI_CSF_NONE){
            metadata->textColIndexes.append(i);
        }
    }

    ref();
}

Resultset::~Resultset()
{   
    stmt->removeResultset(this);

    //if(removedFromStatement && stmt->rsCount()==0){
    //    delete stmt;
    //}

    if(acquiredMutex){
        cout << "RESULTSET COULD BE DELETED WITHOUT RELEASING MUTEX" << endl;
        endFetchRows();
    }

    cout << "released resultset" << endl;

    deref();
}

void Resultset::beginFetchRows()
{
    cn->lock();
    acquiredMutex=true;
}

bool Resultset::moveNext()
{
    Q_ASSERT(acquiredMutex);

    int moveResult=OCI_FetchNext(ociResultset);
    if(!moveResult){
        reachedEOF=true;
        checkForError();
    }

    return (moveResult!=0);
}

bool Resultset::moveToPosition(unsigned int pos)
{
    Q_ASSERT(acquiredMutex);

    int moveResult=OCI_FetchSeek(ociResultset, OCI_SFD_ABSOLUTE, pos);

    if(!moveResult){
        checkForError();
    }

    return (moveResult!=0);
}

bool Resultset::moveToLastFetched()
{
    Q_ASSERT(acquiredMutex);

    unsigned int rowCount = OCI_GetRowCount(ociResultset);
    if(rowCount==0){
        return false;
    }else{
        return moveToPosition(rowCount);
    }
}

void Resultset::checkForError()
{
    try{
        DbUtil::checkForOciError();
    }catch(OciException &ex){
        endFetchRows();
        throw ex;
    }
}

OraExp::ColumnDataType Resultset::convertColumnDataType(unsigned int ociDataType) const
{
    OraExp::ColumnDataType result;

    switch(ociDataType){
    case OCI_CDT_NUMERIC:
        result = OraExp::CDTNumeric;
        break;
    case OCI_CDT_DATETIME:
        result = OraExp::CDTDateTime;
        break;
    case OCI_CDT_TEXT:
        result = OraExp::CDTText;
        break;
    case OCI_CDT_LONG:
        result = OraExp::CDTLong;
        break;
    case OCI_CDT_CURSOR:
        result = OraExp::CDTCursor;
        break;
    case OCI_CDT_LOB:
        result = OraExp::CDTLob;
        break;
    case OCI_CDT_FILE:
        result = OraExp::CDTFile;
        break;
    case OCI_CDT_TIMESTAMP:
        result = OraExp::CDTTimestamp;
        break;
    case OCI_CDT_INTERVAL:
        result = OraExp::CDTInterval;
        break;
    case OCI_CDT_RAW:
        result = OraExp::CDTRaw;
        break;
    case OCI_CDT_OBJECT:
        result = OraExp::CDTObject;
        break;
    case OCI_CDT_COLLECTION:
        result = OraExp::CDTCollection;
        break;
    case OCI_CDT_REF:
        result = OraExp::CDTRef;
        break;
    default:
        result = OraExp::CDTUnknown;
        break;
    }

    return result;
}

OraExp::ColumnSubType Resultset::convertColumnSubType(unsigned int colIx, unsigned int ociSubType) const
{
    OraExp::ColumnSubType result = OraExp::CSTUnknown;
    OraExp::ColumnDataType colDataType = columnMetadata->columnDataTypes.value(colIx);

    switch(colDataType){
    case OraExp::CDTLong:
    {
        switch(ociSubType){
        case OCI_BLONG:
            result = OraExp::CSTBLong;
            break;
        case OCI_CLONG:
            result = OraExp::CSTCLong;
            break;
        }
    }
        break;
    case OraExp::CDTLob:
    {
        switch(ociSubType){
        case OCI_BLOB:
            result = OraExp::CSTBlob;
            break;
        case OCI_CLOB:
            result = OraExp::CSTClob;
            break;
        case OCI_NCLOB:
            result = OraExp::CSTNClob;
            break;
        }
    }
        break;
    case OraExp::CDTFile:
    {
        switch(ociSubType){
        case OCI_BFILE:
            result = OraExp::CSTBFile;
            break;
        case OCI_CFILE:
            result = OraExp::CSTCFile;
            break;
        }
    }
        break;
    case OraExp::CDTTimestamp:
    {
        switch(ociSubType){
        case OCI_TIMESTAMP:
            result = OraExp::CSTTimestamp;
            break;
        case OCI_TIMESTAMP_TZ:
            result = OraExp::CSTTimestampTz;
            break;
        case OCI_TIMESTAMP_LTZ:
            result = OraExp::CSTTimestampLtz;
            break;
        }
    }
        break;
    case OraExp::CDTInterval:
    {
        switch(ociSubType){
        case OCI_INTERVAL_YM:
            result = OraExp::CSTIntervalYm;
            break;
        case OCI_INTERVAL_DS:
            result = OraExp::CSTIntervalDs;
            break;
        }
    }
        break;
    default:
        result = OraExp::CSTUnknown;
        break;
    }

    return result;
}

void Resultset::endFetchRows()
{
    Q_ASSERT(acquiredMutex);

    acquiredMutex=false;
    cn->unlock();
}

int Resultset::getColumnCount() const
{
    return columnMetadata->getColumnCount();
}

bool Resultset::isEOF() const
{
    return reachedEOF;
}

unsigned int Resultset::getColumnIndexByName(const QString &colName) const
{
    return columnMetadata->getColumnIndexByName(colName);
}

QStringList Resultset::getColumnTitles() const
{
    return columnMetadata->columnTitles;
}

QString Resultset::getString(unsigned int colIx) const
{
    if(isNull(colIx)){
        return "";
    }else{
        return toQString(OCI_GetString(ociResultset, colIx));
    }
}

void Resultset::getRaw(unsigned int colIx, void *buffer, unsigned int length) const
{
    OCI_GetRaw(ociResultset, colIx, buffer, length);
}

int Resultset::getInt(unsigned int colIx) const
{
    return OCI_GetInt(ociResultset, colIx);
}

double Resultset::getDouble(unsigned int colIx) const
{
    return OCI_GetDouble(ociResultset, colIx);
}

bool Resultset::isNull(unsigned int colIx) const
{
    return OCI_IsNull(ociResultset, colIx);
}

QString Resultset::getNumberAsString(unsigned int colIx) const
{
    return getAsString(colIx);
}

QString Resultset::getAsString(unsigned int colIx) const
{
    if(!columnMetadata->columnDataTypes.contains(colIx)){
        return "column not found";
    }

    if(isNull(colIx)){
        return "";
    }

    unsigned int dataType = columnMetadata->columnDataTypes.value(colIx);

    QString result;

    if(dataType==OraExp::CDTNumeric || columnMetadata->isTextColumn(colIx)){
        result=toQString(OCI_GetString(ociResultset, colIx));
    }else if(dataType==OraExp::CDTCursor){
        result=QObject::tr("[Cursor]");
    }/*else if(dataType==OraExp::CDTObject){
        result=QObject::tr("[Object]");
    }else if(dataType==OraExp::CDTCollection){
        result=QObject::tr("[Collection]");
    }else if(dataType==OraExp::CDTRaw){
        result=QObject::tr("[Binary data (RAW)]");
    }*/else if(dataType==OraExp::CDTLob && !isTextColumn(colIx)){
        result=QObject::tr("[BLOB]");
    }else if(dataType==OraExp::CDTLong && !isTextColumn(colIx)){
        result=QObject::tr("[LONG RAW]");
    /*}else if(dataType==OraExp::CDTFile && !isTextColumn(colIx)){
        result=QObject::tr("[FILE]");*/
    }else if(dataType==OraExp::CDTDateTime){
        mtext *dateBuffer = new mtext[mtslen(W_DB_DATE_FORMAT)+1];
        OCI_DateToText(OCI_GetDate(ociResultset, colIx),
                       W_DB_DATE_FORMAT,
                       mtslen(W_DB_DATE_FORMAT),
                       dateBuffer);
        result = toQString(dateBuffer);
        delete[] dateBuffer;
    }else if(dataType==OraExp::CDTTimestamp){
        OraExp::ColumnSubType subType = columnMetadata->columnSubTypes.value(colIx);
        mtext *tzBuffer = new mtext[51];
        if(subType==OraExp::CSTTimestampTz){
            OCI_TimestampToText(OCI_GetTimestamp(ociResultset, colIx),
                                W_DB_TZ_TIMESTAMP_FORMAT,
                                50,
                                tzBuffer,
                                9);
        }else{
            OCI_TimestampToText(OCI_GetTimestamp(ociResultset, colIx),
                                W_DB_TIMESTAMP_FORMAT,
                                50,
                                tzBuffer,
                                9);
        }

        result = toQString(tzBuffer);

        delete[] tzBuffer;
    }else{
        result=toQString(OCI_GetString(ociResultset, colIx));
    }

    return result;
}

QString Resultset::fetchOneAsString(unsigned int colIx)
{   
    bool statementHasLockOnConnection = this->stmt->hasLockOnConnection();
    if(statementHasLockOnConnection){
        this->stmt->unlockConnection();
    }

    this->beginFetchRows();
    this->moveNext();
    QString result = getAsString(colIx);
    this->endFetchRows();

    if(statementHasLockOnConnection){
        this->stmt->lockConnection();
    }

    return result;
}

OCI_Date *Resultset::getDate(unsigned int colIx) const
{
    return OCI_GetDate(ociResultset, colIx);
}

OCI_Timestamp *Resultset::getTimestamp(unsigned int colIx) const
{
    return OCI_GetTimestamp(ociResultset, colIx);
}

OCI_Interval *Resultset::getInterval(unsigned int colIx) const
{
    return OCI_GetInterval(ociResultset, colIx);
}

bool Resultset::isTextColumn(unsigned int colIx) const
{
    return columnMetadata->isTextColumn(colIx);
}

bool Resultset::isScrollable() const
{
    return this->stmt->getUseScrollableResultsets();
}

void Resultset::printObjectCount()
{
#ifdef DEBUG
    qDebug() << "Total number of resultsets:" << (int)Resultset::objectCount;
#endif
}

QSharedPointer<ResultsetColumnMetadata> Resultset::getColumnMetadata() const
{
    return this->columnMetadata;
}

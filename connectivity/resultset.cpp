#include "resultset.h"
#include "connection.h"
#include "statement.h"
#include <iostream>
#include "util/strutil.h"
#include "util/dbutil.h"
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
    for(unsigned int i=1; i<=colCount; ++i){
        column=OCI_GetColumn(ociResultset, i);
        columnName=toQString(OCI_GetColumnName(column));
        if(metadata->columnIndexes.contains(columnName)){
            columnName = addNumericSuffix(columnName, metadata->columnIndexes.keys());
        }

        metadata->columnIndexes.insert(columnName, i);
        metadata->columnDataTypes.insert(i, convertColumnDataType(OCI_GetColumnType(column)));

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

QHash<QString, unsigned int> Resultset::getColumnIndexes() const
{
    return columnMetadata->columnIndexes;
}

QString Resultset::getString(unsigned int colIx) const
{
    if(isNull(colIx)){
        return "";
    }else{
        return toQString(OCI_GetString(ociResultset, colIx));
    }
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

    if(dataType==OraExp::CDTCursor){
        result=QObject::tr("Cursor");
    }else if(dataType==OraExp::CDTObject){
        result=QObject::tr("Object");
    }else if(dataType==OraExp::CDTCollection){
        result=QObject::tr("Collection");
    }else if(dataType==OraExp::CDTLob && !isTextColumn(colIx)){
        result=QObject::tr("BLOB");
    }else if(dataType==OraExp::CDTLong && !isTextColumn(colIx)){
        result=QObject::tr("LONG RAW");
    }else if(dataType==OraExp::CDTFile && !isTextColumn(colIx)){
        result=QObject::tr("FILE");
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

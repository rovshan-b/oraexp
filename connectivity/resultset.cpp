#include "resultset.h"
#include "connection.h"
#include "statement.h"
#include <iostream>
#include "util/strutil.h"
#include "util/dbutil.h"
#include <stdexcept>
#include <sstream>
#include <QThread>
#include <QDebug>

using namespace std;

#ifdef DEBUG
QAtomicInt Resultset::objectCount;
#endif

Resultset::Resultset(OCI_Resultset *ociResultset, Connection *cn, Statement *stmt) :
    cn(cn), stmt(stmt), reachedEOF(false), acquiredMutex(false)
{
    this->ociResultset=ociResultset;

    unsigned int colCount = OCI_GetColumnCount(ociResultset);
    OCI_Column *column;
    QString columnName;
    for(unsigned int i=1; i<=colCount; ++i){
        column=OCI_GetColumn(ociResultset, i);
        columnName=toQString(OCI_GetColumnName(column));
        if(columnIndexes.contains(columnName)){
            columnName = addNumericSuffix(columnName, columnIndexes.keys());
        }
        columnIndexes.insert(columnName, i);
        columnDataTypes.insert(i, OCI_GetColumnType(column));

        if(OCI_ColumnGetCharsetForm(column)!=OCI_CSF_NONE){
            textColIndexes.append(i);
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

void Resultset::endFetchRows()
{
    acquiredMutex=false;
    cn->unlock();
}

int Resultset::getColumnCount() const
{
    return columnIndexes.size();
}

bool Resultset::isEOF() const
{
    return reachedEOF;
}

unsigned int Resultset::getColumnIndexByName(const QString &colName) const
{
    if(columnIndexes.contains(colName)){
        return columnIndexes.value(colName);
    }else{
        throw logic_error(QObject::tr("Column does not exist in resultset - %1").arg(colName).toStdString());
    }
}

QHash<QString, unsigned int> Resultset::getColumnIndexes() const
{
    return columnIndexes;
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
    if(!columnDataTypes.contains(colIx)){
        return "column not found";
    }

    if(isNull(colIx)){
        return "";
    }

    unsigned int dataType = columnDataTypes.value(colIx);

    QString result;

    if(dataType==OCI_CDT_CURSOR){
        result=QObject::tr("Cursor");
    }else if(dataType==OCI_CDT_OBJECT){
        result=QObject::tr("Object");
    }else if(dataType==OCI_CDT_COLLECTION){
        result=QObject::tr("Collection");
    }else if(dataType==OCI_CDT_LOB && !isTextColumn(colIx)){
        result=QObject::tr("BLOB");
    }else if(dataType==OCI_CDT_LONG && !isTextColumn(colIx)){
        result=QObject::tr("LONG RAW");
    }else if(dataType==OCI_CDT_FILE && !isTextColumn(colIx)){
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
    return textColIndexes.contains(colIx);
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

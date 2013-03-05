#ifndef RESULTSET_H
#define RESULTSET_H

#include <QHash>
#include <QDateTime>
#include "util/datetimewithtimezone.h"
#include "enums.h"
#include "beans/resultsetcolumnmetadata.h"
#include <ocilib.h>

struct OCI_Resultset;
class Statement;
class Connection;

class Resultset
{
public:
    Resultset(OCI_Resultset *ociResultset, Connection *cn, Statement *stmt);
    ~Resultset();

    Statement *getStatement() const {return this->stmt;}

    void beginFetchRows();
    bool moveNext();
    bool moveToPosition(unsigned int pos);
    bool moveToLastFetched();
    void endFetchRows();

    int getColumnCount() const;
    bool isEOF() const;

    unsigned int getColumnIndexByName(const QString &colName) const;

    QHash<QString, unsigned int> getColumnIndexes() const;

    bool isNull(unsigned int colIx) const;

    QString getString(unsigned int colIx) const;

    int getInt(unsigned int colIx) const;

    double getDouble(unsigned int colIx) const;

    QString getNumberAsString(unsigned int colIx) const;

    QString getAsString(unsigned int colIx) const;

    QString fetchOneAsString(unsigned int colIx);

    OCI_Date *getDate(unsigned int colIx) const;
    OCI_Timestamp *getTimestamp(unsigned int colIx) const;
    OCI_Interval *getInterval(unsigned int colIx) const;

    bool isTextColumn(unsigned int colIx) const;

    bool isScrollable() const;

    static void printObjectCount();

    QSharedPointer<ResultsetColumnMetadata> getColumnMetadata() const;

private:
    OCI_Resultset *ociResultset;

    Connection *cn;
    Statement *stmt;

    QSharedPointer<ResultsetColumnMetadata> columnMetadata;

    int fetchedRowCount;
    bool reachedEOF;
    bool acquiredMutex;

    void checkForError();
    OraExp::ColumnDataType convertColumnDataType(unsigned int ociDataType) const;


#ifdef DEBUG
    static QAtomicInt objectCount;
#endif

    void ref()
    {
#ifdef DEBUG
        Resultset::objectCount.ref();
#endif
    }

    void deref()
    {
#ifdef DEBUG
        Resultset::objectCount.deref();
#endif
    }
};

#endif // RESULTSET_H

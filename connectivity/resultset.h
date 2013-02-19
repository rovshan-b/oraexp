#ifndef RESULTSET_H
#define RESULTSET_H

#include <QHash>
#include <QDateTime>
#include "util/datetimewithtimezone.h"
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

    static void printObjectCount();

private:
    OCI_Resultset *ociResultset;

    Connection *cn;
    Statement *stmt;

    QHash<QString, unsigned int> columnIndexes;
    QHash<int, unsigned int> columnDataTypes;

    int fetchedRowCount;
    bool reachedEOF;
    bool acquiredMutex;


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

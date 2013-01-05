#ifndef PARSINGTABLE_H
#define PARSINGTABLE_H

#include <QStringList>
#include "../grammar_parser/parsingtableaction.h"
#include "../grammar_parser/parsingtablerow.h"

class ParsingTable
{
public:
    QStringList keywords;
    QList<ParsingTableRow *> rows;

    virtual ~ParsingTable();

    int getKeywordIx(const QString &possibleKeyword) const;

protected:
    ParsingTable();
};

#endif // PARSINGTABLE_H

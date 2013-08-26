#ifndef PARSINGTABLE_H
#define PARSINGTABLE_H

#include <QStringList>
#include "../grammar_parser/parsingtableaction.h"
#include "../grammar_parser/parsingtablerow.h"

class ParsingTable
{
public:
    QStringList keywords;
    QStringList ruleNames;
    QList<ParsingTableRow *> rows;

    virtual ~ParsingTable();

    QStringList getKeywords() const;
    int getKeywordIx(const QString &possibleKeyword) const;
    QString getRuleName(int ruleId);

protected:
    ParsingTable();
};

#endif // PARSINGTABLE_H

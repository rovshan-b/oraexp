#ifndef PARSINGTABLE_H
#define PARSINGTABLE_H

#include <QStringList>
#include "../grammar_parser/parsingtableaction.h"
#include "../grammar_parser/parsingtablerow.h"
#include "../grammar_parser/bnfruleoption.h"

class ParsingTable
{
public:
    QHash<QString,int> tokenNames;
    QStringList keywords;
    QStringList reservedWords;
    QStringList ruleNames;
    QHash<int,BNFRuleOption*> ruleOptions;
    QList<ParsingTableRow *> rows;

    virtual ~ParsingTable();

    //QStringList getKeywords() const;
    int getTokenByName(const QString &tokenName) const;
    int getKeywordIx(const QString &possibleKeyword) const;
    bool isKeyword(const QString &word) const {return getKeywordIx(word)!=-1;}
    QString getKeywordLexeme(int ix) const;
    bool isReservedWord(const QString &word) const;
    QString getRuleName(int ruleId);

protected:
    ParsingTable();
};

#endif // PARSINGTABLE_H

#include "parsingtable.h"
#include "../util/strutil.h"

ParsingTable::ParsingTable()
{
}

ParsingTable::~ParsingTable()
{
    qDeleteAll(ruleOptions.values());
    qDeleteAll(rows);
}

int ParsingTable::getTokenByName(const QString &tokenName) const
{
    int token = tokenNames.value(tokenName, -1);

    Q_ASSERT(token != -1);

    return token;
}

/*
QStringList ParsingTable::getKeywords() const
{
    return this->keywords;
}*/

int ParsingTable::getKeywordIx(const QString &possibleKeyword) const
{
    QStringList::const_iterator it=qBinaryFind(keywords.begin(),
                                               keywords.end(),
                                               possibleKeyword,
                                               caseInsensitiveLessThan);
    if(it==keywords.end()){
        return -1;
    }

    int ix = it-keywords.begin();

    return ix;
}

QString ParsingTable::getKeywordLexeme(int ix) const
{
    Q_ASSERT(ix>=0 && ix<=keywords.size());

    return keywords[ix];
}

bool ParsingTable::isReservedWord(const QString &word) const
{
    QStringList::const_iterator it=qBinaryFind(reservedWords.begin(),
                                               reservedWords.end(),
                                               word,
                                               caseInsensitiveLessThan);
    if(it==reservedWords.end()){
        //return -1;
        return false;
    }

    //int ix = it-reservedWords.begin();

    //return ix;

    return true;
}

QString ParsingTable::getRuleName(int ruleId)
{
    //ruleId-s are 1 based
    Q_ASSERT(ruleNames.size() >= ruleId);

    return ruleNames.at(ruleId - 1);
}

#include "parsingtable.h"
#include "../util/strutil.h"

ParsingTable::ParsingTable()
{
}

ParsingTable::~ParsingTable()
{
    qDeleteAll(rows);
}

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

QString ParsingTable::getRuleName(int ruleId)
{
    //ruleId-s are 1 based
    Q_ASSERT(ruleNames.size() >= ruleId);

    return ruleNames.at(ruleId - 1);
}

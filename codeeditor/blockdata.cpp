#include "blockdata.h"
#include "beans/tokeninfo.h"

BlockData::BlockData()
{
}

BlockData::~BlockData()
{
    clear();
}

void BlockData::addToken(TokenInfo *tokenInfo)
{
    tokens.append(tokenInfo);
}

void BlockData::clear()
{
    qDeleteAll(tokens);
    tokens.clear();
}

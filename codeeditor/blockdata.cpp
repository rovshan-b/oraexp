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

TokenInfo *BlockData::tokenAtPosition(int position) const
{
    foreach(TokenInfo *token, tokens){
        if(position >= token->startPos &&
                position <= token->endPos){
            return token;
        }
    }

    return 0;
}

TokenInfo *BlockData::firstTokenFor(int position) const
{
    foreach(TokenInfo *token, tokens){
        if(token->startPos >= position){
            return token;
        }
    }

    return 0;
}

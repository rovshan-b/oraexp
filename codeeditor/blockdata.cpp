#include "blockdata.h"
#include "beans/tokeninfo.h"
#include "code_parser/plsql/plsqlparsingtable.h"

BlockData::BlockData() :
    //sectionSeparator(false),
    collapseEndLine(-1),
    versionId(-1),
    collapsedUntilLineNumber(-1)
{
}

BlockData::~BlockData()
{
    clear();
}

void BlockData::addToken(TokenInfo *tokenInfo)
{
    tokens.append(tokenInfo);

    //static int tProcedure = PlSqlParsingTable::getInstance()->getKeywordIx("PROCEDURE");
    //static int tFunction = PlSqlParsingTable::getInstance()->getKeywordIx("FUNCTION");

    //if(tokenInfo->tokenOrRuleId == tProcedure ||
    //        tokenInfo->tokenOrRuleId == tFunction){
    //    this->sectionSeparator = true;
    //}
}

void BlockData::clear()
{
    qDeleteAll(tokens);
    tokens.clear();

    //sectionSeparator = false;
}

TokenInfo *BlockData::tokenAtPosition(int position) const
{
    int tokenCount = tokens.count();
    for(int i=0; i<tokenCount; ++i){
        TokenInfo *token = tokens[i];

        if(position >= token->startPos &&
                (position < token->endPos || //given position is less than token end position
                 (i+1 < tokenCount && tokens[i+1]->startPos > position && position <= token->endPos) || //next token's start position is more than position but current token's end position is same as position
                 (i == tokenCount - 1 && position <= token->endPos )) //current token is last token and its end position is same as position
                ){

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

void BlockData::setCollapsePosition(int endLine, int versionId)
{
    this->collapseEndLine = endLine;
    this->versionId = versionId;
}

void BlockData::setCollapsedUntil(int blockNumber)
{
    this->collapsedUntilLineNumber = blockNumber;
}

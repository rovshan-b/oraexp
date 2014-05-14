#include "blockdata.h"
#include "beans/tokeninfo.h"
#include "code_parser/plsql/plsqlparsingtable.h"

BlockData::BlockData() :
    sectionSeparator(false),
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

    if(PlSqlParsingTable::getInstance()->isSectionSeparator(tokenInfo->tokenOrRuleId)){
        this->sectionSeparator = true;
    }
}

void BlockData::clear()
{
    qDeleteAll(tokens);
    tokens.clear();

    sectionSeparator = false;
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

void BlockData::setCollapsePosition(int endLine, int versionId)
{
    this->collapseEndLine = endLine;
    this->versionId = versionId;
}

void BlockData::setCollapsedUntil(int blockNumber)
{
    this->collapsedUntilLineNumber = blockNumber;
}

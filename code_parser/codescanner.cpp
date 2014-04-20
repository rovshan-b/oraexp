#include "codescanner.h"
#include "beans/tokeninfo.h"

CodeScanner::CodeScanner(TextReaderBase *textReader, bool deleteReader) :
    textReader(textReader),
    deleteReader(deleteReader)
{
    init();
}

CodeScanner::CodeScanner() :
    textReader(0),
    deleteReader(false)
{
    init();
}

CodeScanner::~CodeScanner()
{
    if(deleteReader){
        delete textReader;
    }
}

void CodeScanner::init()
{
    tokenStartPos = 0;
    tokenEndPos = 0;
    tokenStartLine = 0;
    tokenEndLine = 0;
    tokenStartLinePos = 0;
    tokenEndLinePos = 0;
}

void CodeScanner::setTextReader(TextReaderBase *textReader, bool deleteReader)
{
    if(this->deleteReader && this->textReader){
        delete this->textReader;
    }

    this->textReader = textReader;
    this->deleteReader = deleteReader;

    //reset
    init();
}

int CodeScanner::getCurrPos() const
{
    return textReader->getCurrPos();
}

TokenInfo *CodeScanner::createTokenInfo(int token, bool includeLexeme) const
{
    TokenInfo *ti = new TokenInfo();
    ti->tokenType = TokenInfo::Token;
    ti->tokenOrRuleId = token;
    ti->startPos = getTokenStartPos();
    ti->endPos = getTokenEndPos();
    ti->startLine = getTokenStartLine();
    ti->endLine = getTokenEndLine();
    ti->startLinePos = getTokenStartLinePos();
    ti->endLinePos = getTokenEndLinePos();
    if(includeLexeme){
        ti->lexeme = getTokenLexeme();
    }

    return ti;
}

QChar CodeScanner::getNextChar()
{
    QChar c = textReader->getNextCharacter();

    return c;
}

void CodeScanner::ungetChar()
{
    if(c.isNull()){
        return;
    }

    textReader->ungetCharacter();
}

/*
bool CodeScanner::isEOF() const
{
    return textReader->isEOF();
}*/

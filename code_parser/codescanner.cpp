#include "codescanner.h"

CodeScanner::CodeScanner(TextReaderBase *textReader) :
    textReader(textReader),
    tokenStartPos(0),
    tokenEndPos(0),
    tokenStartLine(0),
    tokenEndLine(0),
    tokenStartLinePos(0),
    tokenEndLinePos(0)
{
}

CodeScanner::~CodeScanner()
{
    delete textReader;
}

int CodeScanner::getCurrPos() const
{
    return textReader->getCurrPos();
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

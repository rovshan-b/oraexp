#include "codescanner.h"
#include "itextreader.h"

CodeScanner::CodeScanner(ITextReader *textReader) :
    textReader(textReader),
    currPos(0),
    currLineNo(0),
    currLinePos(0)
{
}

CodeScanner::~CodeScanner()
{
    delete textReader;
}

QChar CodeScanner::getNextChar()
{
    return textReader->getNextCharacter();
}

void CodeScanner::ungetChar()
{
    textReader->ungetCharacter();
}

bool CodeScanner::isEOF() const
{
    return textReader->isEOF();
}

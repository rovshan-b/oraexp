#ifndef CODESCANNER_H
#define CODESCANNER_H

#define NON_LITERAL_START_IX 100000

#include <QString>
#include "textreaderbase.h"

class CodeScanner
{
public:
    CodeScanner(TextReaderBase *textReader);
    virtual ~CodeScanner();

    virtual int getNextToken(bool skipWhitespace = true)=0;

    int getTokenStartPos() const {return tokenStartPos;}
    int getTokenEndPos() const {return tokenEndPos;}
    int getTokenStartLine() const {return tokenStartLine;}
    int getTokenEndLine() const {return tokenEndLine;}
    int getTokenStartLinePos() const {return tokenStartLinePos;}
    int getTokenEndLinePos() const {return tokenEndLinePos;}

    int getCurrPos() const;

    QString getTokenLexeme() const {return currentLexeme;}

    bool isNewline() const {return c=='\n' || c==0x2029;}

    QString getCurrentLine() const {return textReader->getCurrentLine();}

protected:
    TextReaderBase *textReader;

    int tokenStartPos;
    int tokenEndPos;
    int tokenStartLine;
    int tokenEndLine;
    int tokenStartLinePos;
    int tokenEndLinePos;

    QChar c;

    QString currentLexeme;

    QChar getNextChar();
    void ungetChar();
    //bool isEOF() const;
};

#endif // CODESCANNER_H

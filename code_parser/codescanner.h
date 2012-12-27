#ifndef CODESCANNER_H
#define CODESCANNER_H

class TextReaderBase;

#include <QChar>

class CodeScanner
{
public:
    CodeScanner(TextReaderBase *textReader);
    virtual ~CodeScanner();

    virtual int getNextToken()=0;

    int getTokenStartPos(){return tokenStartPos;}
    int getTokenEndPos(){return tokenEndPos;}
    int getTokenStartLine(){return tokenStartLine;}
    int getTokenEndLine(){return tokenEndLine;}
    int getTokenStartLinePos(){return tokenStartLinePos;}
    int getTokenEndLinePos(){return tokenEndLinePos;}

    bool isNewline() const {return c=='\n' || c==0x2029;}

protected:
    TextReaderBase *textReader;

    int tokenStartPos;
    int tokenEndPos;
    int tokenStartLine;
    int tokenEndLine;
    int tokenStartLinePos;
    int tokenEndLinePos;

    QChar c;

    QChar getNextChar();
    void ungetChar();
    bool isEOF() const;
};

#endif // CODESCANNER_H

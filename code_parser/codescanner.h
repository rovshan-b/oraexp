#ifndef CODESCANNER_H
#define CODESCANNER_H

class ITextReader;

#include <QChar>

class CodeScanner
{
public:
    CodeScanner(ITextReader *textReader);
    virtual ~CodeScanner();

    virtual int getNextToken()=0;

    int getCurrPos(){return currPos;}
    int getCurrLineNo(){return currLineNo;}
    int getCurrLinePos(){return currLinePos;}

private:
    ITextReader *textReader;

protected:
    int currPos;
    int currLineNo;
    int currLinePos;

    int tokenPos;
    int tokenLineNo;
    int tokenLinePos;

    QChar getNextChar();
    void ungetChar();
    bool isEOF() const;
};

#endif // CODESCANNER_H

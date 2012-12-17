#ifndef EBNFSCANNER_H
#define EBNFSCANNER_H

#include "ebnftoken.h"

class QTextStream;

class EBNFScanner
{
public:
    EBNFScanner(QTextStream *stream);

    EBNFToken getToken();

    enum EBNFScannerState {
        START,
        INID,
        INTERMINAL,
        INESCAPECHAR,
        INRANGE,
        DONE
    };

    int getCurrPos(){return currPos;}
    int getCurrLineNo(){return currLineNo;}
    int getCurrLinePos(){return currLinePos;}

    static EBNFToken createEOFToken();

private:
    QChar getNextChar();
    void ungetChar();
    QTextStream *stream;

    int currPos;
    int currLineNo;
    int currLinePos;
};

#endif // EBNFSCANNER_H

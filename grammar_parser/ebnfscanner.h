#ifndef EBNFSCANNER_H
#define EBNFSCANNER_H

class QTextStream;
#include "ebnftoken.h"

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

private:
    QChar getNextChar();
    void ungetChar();
    QTextStream *stream;

    int currPos;
    int currLineNo;
    int currLinePos;
};

#endif // EBNFSCANNER_H

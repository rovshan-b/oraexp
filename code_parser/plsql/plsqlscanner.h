#ifndef PLSQLSCANNER_H
#define PLSQLSCANNER_H

#include "../codescanner.h"

class PlSqlScanner : public CodeScanner
{
public:
    PlSqlScanner(ITextReader *textReader);

    int getNextToken();

private:
    enum ScannerState {
        START,
        INID,
        INTERMINAL,
        INCOMMENT,
        DONE
    };
};

#endif // PLSQLSCANNER_H

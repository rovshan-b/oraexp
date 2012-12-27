#ifndef PLSQLSCANNER_H
#define PLSQLSCANNER_H

#include "../codescanner.h"
#include <QString>

class PlSqlScanner : public CodeScanner
{
public:
    PlSqlScanner(TextReaderBase *textReader);

    int getNextToken();

    QString currentLexeme;

private:
    enum ScannerState {
        START,
        IN_ID,
        IN_DOUBLE_DOT,
        IN_EXPONENT,
        IN_LLABEL,
        IN_RLABEL,
        IN_EQ,
        IN_ASSIGN,
        IN_DOUBLE_VERTBAR,
        IN_NOT_EQ,
        IN_QUOTED_STRING,
        IN_QUOTED_STRING_END,
        IN_DOUBLEQUOTED_STRING,
        IN_MINUS,
        IN_SL_COMMENT,
        IN_DIVIDE,
        IN_ML_COMMENT,
        IN_ML_COMMENT_END,
        IN_NUMBER,
        IN_NUMBER_DECIMAL_PART,
        IN_NUMBER_EXP_PART,
        IN_NUMBER_END,
        DONE
    };
};

#endif // PLSQLSCANNER_H

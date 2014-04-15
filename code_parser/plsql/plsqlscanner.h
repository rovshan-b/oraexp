#ifndef PLSQLSCANNER_H
#define PLSQLSCANNER_H

#include "../codescanner.h"
#include <QString>

class ParsingTable;

class PlSqlScanner : public CodeScanner
{
public:
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
        IN_ID_OR_QUOTED_STRING,
        IN_QUOTED_STRING,
        IN_QUOTED_STRING_END,
        IN_SPECIAL_QUOTED_STRING,
        IN_SPECIAL_QUOTED_STRING_END,
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

    PlSqlScanner(TextReaderBase *textReader, bool deleteReader = true);
    PlSqlScanner();

    void init();

    virtual int getNextToken(bool skipWhitespace = true);

    inline static bool isIdCharacter(const QChar &c) {return c.isLetterOrNumber() || c=='_' || c=='$' || c=='#';}

    void setInitialState(PlSqlScanner::ScannerState state) {this->initialState = state;}
    PlSqlScanner::ScannerState getEndState() const {return this->endState;}

private:
    int doGetNextToken();

    ParsingTable *parsingTable;
    PlSqlScanner::ScannerState initialState;
    PlSqlScanner::ScannerState endState;
};

#endif // PLSQLSCANNER_H

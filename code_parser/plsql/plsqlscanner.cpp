#include "plsqlscanner.h"
#include "plsqltokens.h"
#include "../textreaderbase.h"
#include "../plsql/plsqlparsingtable.h"
#include <QDebug>

PlSqlScanner::PlSqlScanner(TextReaderBase *textReader) : CodeScanner(textReader)
{
    parsingTable=PlSqlParsingTable::getInstance();
    Q_ASSERT(parsingTable);
}

int PlSqlScanner::getNextToken(bool skipWhitespace)
{
    int token = doGetNextToken();

    if(skipWhitespace){
        while(token==PLS_SL_COMMENT || token==PLS_ML_COMMENT){
            token = doGetNextToken();
        }
    }

    return token;
}

int PlSqlScanner::doGetNextToken()
{
    int token;
    ScannerState state=START;
    bool save;

    currentLexeme.clear();

    tokenStartPos=textReader->getCurrPos();
    tokenStartLine=textReader->getCurrLineNo();
    tokenStartLinePos=textReader->getCurrLinePos();

    while(state!=DONE){
        c=getNextChar();

        if(currentLexeme.isEmpty() && tokenStartLine!=textReader->getCurrLineNo()){
            tokenStartLine=textReader->getCurrLineNo();
            tokenStartLinePos=0;
        }

        save=true;

        switch(state){
        case START:
            if(isNewline()) {save=false; ++tokenStartPos;++tokenStartLine;tokenStartLinePos=0;}
            else if(c.isSpace()) {save=false; ++tokenStartPos;++tokenStartLinePos;}
            else if(c.isLetter()) {state=IN_ID;}
            else {
                state = DONE;

                if(c == ';') token=PLS_SEMI;
                else if(c == ',') token=PLS_COMMA;
                else if(c == '@') token=PLS_AT_SIGN;
                else if(c=='(') token=PLS_LPAREN;
                else if(c == ')') token=PLS_RPAREN;
                else if(c=='[') token=PLS_LBRACK;
                else if(c == ']') token=PLS_RBRACK;
                else if(c=='%') token=PLS_PERCENTAGE;
                else if(c=='+') token=PLS_PLUS;

                else if(c=='.') state=IN_DOUBLE_DOT;
                else if(c=='*') state=IN_EXPONENT;
                else if(c=='<') state=IN_LLABEL;
                else if(c=='>') state=IN_RLABEL;
                else if(c=='=') state=IN_EQ;
                else if(c==':') state=IN_ASSIGN;
                else if(c=='|') state=IN_DOUBLE_VERTBAR;
                else if(c=='!' || c=='^') state=IN_NOT_EQ;
                else if(c=='\'') state=IN_QUOTED_STRING;
                else if(c=='"') state=IN_DOUBLEQUOTED_STRING;
                else if(c=='-') state=IN_MINUS;
                else if(c=='/') state=IN_DIVIDE;
                else if(c.isDigit()) state=IN_NUMBER;

                else if(c.isNull()) token=PLS_E_O_F;
                else token=PLS_ERROR;
            }
            break;
        case IN_ID:
            if((!PlSqlScanner::isIdCharacter(c)) || c.isNull()){
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_ID;
            }
            break;
        case IN_DOUBLE_DOT:
            if(c=='.'){
                state=DONE;
                token=PLS_DOUBLE_DOT;
            }else if(c.isDigit()){
                state=IN_NUMBER_DECIMAL_PART;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_DOT;
            }
            break;
        case IN_EXPONENT:
            if(c=='*'){
                state=DONE;
                token=PLS_EXPONENT;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_ASTERISK;
            }
            break;
        case IN_LLABEL:
            if(c=='<'){
                state=DONE;
                token=PLS_LLABEL;
            }else if(c=='='){
                state=DONE;
                token=PLS_LESS_THAN_EQUAL;
            }else if(c=='>'){
                state=DONE;
                token=PLS_NOT_EQ;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_LESS_THAN;
            }
            break;
        case IN_RLABEL:
            if(c=='>'){
                state=DONE;
                token=PLS_RLABEL;
            }else if(c=='='){
                state=DONE;
                token=PLS_GREATER_THAN_EQUAL;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_GREATER_THAN;
            }
            break;
        case IN_EQ:
            if(c=='>'){
                state=DONE;
                token=PLS_ARROW;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_EQ;
            }
            break;
        case IN_ASSIGN:
            if(c=='='){
                state=DONE;
                token=PLS_ASSIGN;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_COLON;
            }
            break;
        case IN_DOUBLE_VERTBAR:
            if(c=='|'){
                state=DONE;
                token=PLS_DOUBLEVERTBAR;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_VERTBAR;
            }
            break;
        case IN_NOT_EQ:
            if(c=='='){
                state=DONE;
                token=PLS_NOT_EQ;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_ERROR;
            }
            break;
        case IN_QUOTED_STRING:
            if(c=='\''){
                state=IN_QUOTED_STRING_END;
            }else if(c.isNull()){
                state=DONE;
                save=false;
                token=PLS_ERROR;
            }
            break;
        case IN_QUOTED_STRING_END:
            if(c=='\''){
                state=IN_QUOTED_STRING;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_QUOTED_STRING;
            }
            break;
        case IN_DOUBLEQUOTED_STRING:
            if(c=='"'){
                state=DONE;
                token=PLS_DOUBLEQUOTED_STRING;
            }else if(c.isNull()){
                state=DONE;
                token=PLS_ERROR;
            }
            break;
        case IN_MINUS:
            if(c=='-'){
                state=IN_SL_COMMENT;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_MINUS;
            }
            break;
        case IN_SL_COMMENT:
            if(isNewline() || c.isNull()){
                state=DONE;
                save=false;
                token=PLS_SL_COMMENT;
            }
            break;
        case IN_DIVIDE:
            if(c=='*'){
                state=IN_ML_COMMENT;
                save=false;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_DIVIDE;
            }
            break;
        case IN_ML_COMMENT:
            if(c=='*'){
                state=IN_ML_COMMENT_END;
                save=false;
            }else if(c.isNull()){
                state=DONE;
                token=PLS_ERROR;
            }else{
                save=false;
            }
            break;
        case IN_ML_COMMENT_END:
            if(c=='/'){
                state=DONE;
                token=PLS_ML_COMMENT;
            }else if(c=='*'){
                //stay in this state
                save=false;
            }else{
                state=IN_ML_COMMENT;
                save=false;
            }
            break;
        case IN_NUMBER:
            if(c=='.'){
                state=IN_NUMBER_DECIMAL_PART;
            }else if(c=='E' || c=='e'){
                state=IN_NUMBER_EXP_PART;
            }
            else if(!c.isDigit()){
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_NUMBER;
            }
            break;
        case IN_NUMBER_DECIMAL_PART:
            if(c=='.'){
                ungetChar();
                ungetChar();
                save=false;
                currentLexeme.chop(1);
                state=DONE;
                token=PLS_NUMBER;
            }else if(c=='E' || c=='e'){
                state=IN_NUMBER_EXP_PART;
            }else if(c.isDigit()){
                state=IN_NUMBER_END;
            }else if(!c.isDigit()){
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_NUMBER;
            }
            break;
        case IN_NUMBER_EXP_PART:
            if(c=='+' || c=='-' || c.isDigit()){
                state=IN_NUMBER_END;
            }else{
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_ERROR;
            }
            break;
        case IN_NUMBER_END:
            if(!c.isDigit()){
                ungetChar();
                save=false;
                state=DONE;
                token=PLS_NUMBER;
            }
            break;
        case DONE:
            qDebug() << "entered state DONE in PlSqlScanner, what should not happen";
            state=DONE;
            token=PLS_ERROR;
            break;
        }

        if(save && !c.isNull()){
            currentLexeme.append(c);
        }
    }

    tokenEndPos=textReader->getCurrPos();
    tokenEndLine=textReader->getCurrLineNo();
    tokenEndLinePos=textReader->getCurrLinePos();

    //if we scanned a PLS_ID, check if it is a keyword
    int keywordIx=parsingTable->getKeywordIx(currentLexeme);
    if(keywordIx!=-1){ //it is a keyword
        token = keywordIx;
    }

    return token;
}

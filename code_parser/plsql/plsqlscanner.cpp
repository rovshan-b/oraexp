#include "plsqlscanner.h"

PlSqlScanner::PlSqlScanner(ITextReader *textReader) : CodeScanner(textReader)
{
}

int PlSqlScanner::getNextToken()
{
    int token;
    ScannerState state=START;
    bool save;

    this->tokenPos=this->currPos;
    this->tokenLineNo=this->currLineNo;
    this->tokenLinePos=this->currLinePos;

    while(state!=DONE){
        QChar c=getNextChar();
        save=true;

        switch(state){
        case START:
            if(c=='\n') {save=false; currLineNo++;currLinePos=0;}
            else if(c.isSpace()) {save=false; ++this->tokenPos;}
            else if(c.isLetter() && c.isLower()) {state=INID;}
            else if(c.isLetter() && c.isUpper()){state=INTERMINAL;isLiteral=false;}
            else if(c=='\''){save=false; state=INTERMINAL;isLiteral=true;}
            else if(c=='.') state=INRANGE;
            else {
                state = DONE;

                if(c == ':') token.tokenType=EBNFToken::COLON;
                else if(c == '|') token.tokenType=EBNFToken::PIPE;
                else if(c=='(') token.tokenType=EBNFToken::LPAREN;
                else if(c == ')') token.tokenType=EBNFToken::RPAREN;
                else if(c== ';') token.tokenType=EBNFToken::SEMICOLON;
                else if(c=='~') token.tokenType=EBNFToken::NEGATION;
                else if(c=='+') token.tokenType=EBNFToken::PLUS;
                else if(c=='*') token.tokenType=EBNFToken::STAR;
                else if(c=='?') token.tokenType=EBNFToken::QUESTION;
                else if(c.isNull()) {token.tokenType=EBNFToken::E_O_F;token.isLiteralTerminal=false;}
                else token.tokenType=EBNFToken::ERR;
            }
            break;
        case INID:
            if(!c.isLetterOrNumber() && c!='_'){
                ungetChar();
                save=false;
                state=DONE;
                token.tokenType=EBNFToken::ID;
            }
            break;
        case INTERMINAL:
            if(c=='\'' && isLiteral){
                save=false;
                state=DONE;
                token.tokenType=EBNFToken::TERMINAL;
                token.isLiteralTerminal=true;
            }else if(c=='\\' && isLiteral){
                save=false;
                state=INESCAPECHAR;
            }else if(!c.isLetterOrNumber() && c!='_' && !isLiteral){
                ungetChar();
                save=false;
                state=DONE;
                token.tokenType=EBNFToken::TERMINAL;
                token.isLiteralTerminal=false;
            }
            break;
        case INESCAPECHAR:
            state=INTERMINAL;
            if(token.lexeme=="n"){
                token.lexeme="__newline__";
            }else if(token.lexeme=="r"){
                token.lexeme="__line_feed__";
            }else if(token.lexeme=="t"){
                token.lexeme="__tab__";
            }
            break;
        case INRANGE:
            state=DONE;
            if(c=='.'){
                token.tokenType=EBNFToken::RANGE;
            }else{
                token.tokenType=EBNFToken::ERR;
            }
            break;
        case DONE:
            qDebug() << "entered state DONE in EBNFScanner, what should not happen";
            state=DONE;
            token.tokenType=EBNFToken::ERR;
            break;
        }

        if(save){
            token.lexeme.append(c);
        }

        //if(state==DONE){
        //
        //}
    }

    token.endPos=currPos;
    token.lineEndPos=currLinePos;

    return token;
}

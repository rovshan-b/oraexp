#include "ebnfscanner.h"
#include <QTextStream>
#include <QtDebug>

EBNFScanner::EBNFScanner(QTextStream *stream) : stream(stream), currPos(0), currLineNo(1), currLinePos(0)
{

}

QChar EBNFScanner::getNextChar()
{
    QChar c;

    if(!stream->atEnd()){
        (*stream) >> c;
        currPos++;
        currLinePos++;
    }

    return c;
}

void EBNFScanner::ungetChar()
{
    stream->seek(stream->pos()-1);
    currPos--;
    currLinePos--;
}

EBNFToken EBNFScanner::getToken()
{
    EBNFToken token;
    EBNFScannerState state=START;
    bool save;

    token.startPos=currPos;
    token.lineNo=currLineNo;
    token.lineStartPos=currLinePos;

    while(state!=DONE){
        QChar c=getNextChar();
        save=true;

        switch(state){
        case START:
            if(c=='\n') {save=false; currLineNo++;currLinePos=0;}
            else if(c.isSpace()) {save=false; token.startPos++;}
            else if(c.isLetter()) {state=INID;}
            else if(c=='\''){save=false; state=INTERMINAL;}
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
                else if(c.isNull()) token.tokenType=EBNFToken::E_O_F;
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
            if(c=='\''){
                save=false;
                state=DONE;
                token.tokenType=EBNFToken::TERMINAL;
            }else if(c=='\\'){
                save=false;
                state=INESCAPECHAR;
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

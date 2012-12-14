#include "ebnftoken.h"

EBNFToken::EBNFToken() : tokenType(ERR),
    isLiteralTerminal(true),
    nonLiteralTerminalDefId(-1),
    startPos(0),
    endPos(0),
    lineNo(0),
    lineStartPos(0),
    lineEndPos(0)
{
}

bool EBNFToken::operator ==(const EBNFToken &other) const
{
    return this->lexeme==other.lexeme && this->tokenType==other.tokenType && this->isLiteralTerminal==other.isLiteralTerminal;
}

QString EBNFToken::toString() const
{
    QString str;
    str.append("Token type = ");
    switch(tokenType){
    case ID:
        str.append("Identifier");
        break;
    case COLON:
        str.append("Colon");
        break;
    case TERMINAL:
        if(isLiteralTerminal){
            str.append("Terminal");
        }else{
            str.append("Non literal terminal");
        }
        break;
    case EPSILON:
        str.append("Epsilon");
        break;
    case PIPE:
        str.append("Pipe");
        break;
    case LPAREN:
        str.append("Left parenthesis");
        break;
    case RPAREN:
        str.append("Right parenthesis");
        break;
    case SEMICOLON:
        str.append("Semicolon");
        break;
    case NEGATION:
        str.append("Negation");
        break;
    case PLUS:
        str.append("Plus");
        break;
    case STAR:
        str.append("STAR");
        break;
    case QUESTION:
        str.append("Question");
        break;
    case RANGE:
        str.append("Range");
        break;
    case E_O_F:
        str.append("End of file");
        break;
    case ERR:
        str.append("Error");
        break;
    case NON_GRAMMAR:
        str.append(lexeme);
        break;
    default:
        str.append("Unknown");
        break;
    }

    str.append(", Lexeme = ").append(lexeme);

    return str;
}

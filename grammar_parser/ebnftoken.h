#ifndef EBNFTOKEN_H
#define EBNFTOKEN_H

#include <QString>

class EBNFToken
{
public:
    EBNFToken();

    bool operator==(const EBNFToken &other) const;

    QString lexeme;

    QString toString() const;

    enum EBNFTokenType {
        ID,
        COLON,
        TERMINAL,
        EPSILON,
        PIPE,
        LPAREN,
        RPAREN,
        SEMICOLON,
        NEGATION,
        PLUS,
        STAR,
        QUESTION,
        RANGE,
        E_O_F,
        ERR,
        NON_GRAMMAR
    };

    EBNFToken::EBNFTokenType tokenType;
    bool isLiteralTerminal; //only for terminals
    int nonLiteralTerminalDefId; //id assigned in #define statement for target parser

    int startPos;
    int endPos;

    int lineNo;
    int lineStartPos;
    int lineEndPos;


};

#endif // EBNFTOKEN_H

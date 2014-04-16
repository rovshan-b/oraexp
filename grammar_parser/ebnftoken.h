#ifndef EBNFTOKEN_H
#define EBNFTOKEN_H

#include <QString>

#define NON_LITERAL_START_IX 100000

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
        OPTIONS_BLOCK,
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

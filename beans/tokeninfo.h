#ifndef TOKENINFO_H
#define TOKENINFO_H

#include <QString>

class TokenInfo
{
public:
    enum TokenType
    {
        Token,
        Rule
    };

    TokenInfo();

    TokenType tokenType;

    int tokenOrRuleId;

    int startPos;
    int endPos;
    int startLine;
    int endLine;
    int startLinePos;
    int endLinePos;

    QString lexeme;

    QString toString() const;
};

#endif // TOKENINFO_H

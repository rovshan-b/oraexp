#ifndef BLOCKDATA_H
#define BLOCKDATA_H

#include <QTextBlockUserData>

class TokenInfo;

class BlockData : public QTextBlockUserData
{
public:
    BlockData();
    virtual ~BlockData();

    void addToken(TokenInfo *tokenInfo);
    void clear();

    //works in block positions (starts from 0 for every block)
    TokenInfo *tokenAtPosition(int position) const;
    TokenInfo *firstTokenFor(int position) const;

private:
    QList<TokenInfo*> tokens;
};

#endif // BLOCKDATA_H

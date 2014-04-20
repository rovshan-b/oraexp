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

private:
    QList<TokenInfo*> tokens;
};

#endif // BLOCKDATA_H

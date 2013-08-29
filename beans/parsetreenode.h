#ifndef PARSETREENODE_H
#define PARSETREENODE_H

#include <QList>

class TokenInfo;

class ParseTreeNode
{
public:
    ParseTreeNode();
    virtual ~ParseTreeNode();

    QList<ParseTreeNode*> children;

    TokenInfo *tokenInfo;

    int symbolCount;
};

#endif // PARSETREENODE_H

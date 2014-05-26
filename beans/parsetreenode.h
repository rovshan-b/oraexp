#ifndef PARSETREENODE_H
#define PARSETREENODE_H

#include <QList>
#include "tokeninfo.h"

#include <QDebug>

class ParseTreeNodeScope;

class ParseTreeNode
{
public:
    ParseTreeNode();
    virtual ~ParseTreeNode();

    QList<ParseTreeNode*> children;

    TokenInfo *tokenInfo;

    int symbolCount;

    ParseTreeNodeScope *scope;
    bool ownsScope;

    inline bool containsPosition(int position){
        return position>=tokenInfo->startPos && position<=tokenInfo->endPos;
    }

};

inline bool parseTreeNodeLessThan(ParseTreeNode *n1, ParseTreeNode *n2)
{
    return n1->tokenInfo->startPos < n2->tokenInfo->startPos;
}

#endif // PARSETREENODE_H
